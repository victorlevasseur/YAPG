#include "Level/LevelState.hpp"

#include <sstream>

#include <SFML/System/Time.hpp>

#include "Collision/CollisionSystem.hpp"
#include "Common/EntityGridSystem.hpp"
#include "Common/PositionComponent.hpp"
#include "Error/ErrorState.hpp"
#include "Health/HealthSystem.hpp"
#include "Level/LevelFailureState.hpp"
#include "Level/LevelSuccessState.hpp"
#include "Lua/CustomBehaviorSystem.hpp"
#include "Menu/MainMenuState.hpp"
#include "Platformer/PlatformerHitboxComponent.hpp"
#include "Platformer/PlatformerSystem.hpp"
#include "Player/PlayerComponent.hpp"
#include "Player/PlayerSystem.hpp"
#include "Rendering/RenderSystem.hpp"
#include "State/StateEngine.hpp"

namespace yapg
{

LevelState::LevelState(StateEngine& stateEngine, std::string path, std::unique_ptr<LuaState>&& luaState, std::unique_ptr<Level>&& level, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager) :
    State(stateEngine),
    m_path(path),
    m_luaState(std::move(luaState)),
    m_level(std::move(level)),
    m_systemMgr(m_level->getEntityManager(), m_level->getEventManager()),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_font(resourcesManager.getFonts().requestResource("assets/LiberationSans.ttf")),
    m_perfText("update: -.----\nrender: -.----", *m_font, 14),
    m_lastUpdateDuration(),
    m_lastRenderDuration(),
    m_gridText("", *m_font, 16),
    m_asyncExecutor()
{
    m_systemMgr.add<EntityGridSystem>();
    auto& grid = m_systemMgr.system<EntityGridSystem>()->getGrid();

    m_systemMgr.add<RenderSystem>(resourcesManager.getTextures(), grid);
    m_systemMgr.add<CustomBehaviorSystem>();
    m_systemMgr.add<CollisionSystem>(grid);
    m_systemMgr.add<PlatformerSystem>(grid);
    m_systemMgr.add<PlayerSystem>(1, settingsManager);
    m_systemMgr.add<HealthSystem>(settingsManager);

    m_systemMgr.configure();

    //Load the players
    //TODO: Support multiple players creation
    std::cout << "Creating players..." << std::endl;

    const EntityTemplate& playerTemplate = m_luaState->getTemplate(m_level->getPlayersTemplates().at(0));

    entityx::Entity playerEntity = m_level->getEntityManager().create();
    playerTemplate.initializeEntity(playerEntity, SerializedEntityGetter());

    //Set x and y parameters according to spawn_position
    auto& parameters = playerTemplate.getParameters();

    const EntityTemplate::Parameter& xParameter = parameters.at("x");
    const EntityTemplate::Parameter& yParameter = parameters.at("y");

    EntityHandle(playerEntity).setAttributeAsAny(xParameter.component, xParameter.attribute, m_level->getSpawnPosition().x);
    EntityHandle(playerEntity).setAttributeAsAny(yParameter.component, yParameter.attribute, m_level->getSpawnPosition().y);

    if(!playerEntity.has_component<PlayerComponent>())
        throw std::runtime_error(std::string("[Level/Error] Player entities must have the \"Player\" component declared in their template ! Not the case with \"") + m_level->getPlayersTemplates()[0] + std::string("\""));
    playerEntity.component<PlayerComponent>()->playerNumber = 0;

    std::cout << "Players created." << std::endl;
    /////////////////////////////////////////

    //Debug info
    m_perfText.setPosition(sf::Vector2f(10.f, 10.f));
    m_perfText.setColor(sf::Color::Black);
    m_gridText.setColor(sf::Color::Black);

    //First update to register the object
    m_systemMgr.update<EntityGridSystem>(0);

    //Put the current level instance into "current_level" lua global variable
    m_luaState->getState().set("current_level", this);
}

void LevelState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelState::render(sf::RenderTarget& target)
{
    auto timeBefore = std::chrono::high_resolution_clock::now();

    target.clear(sf::Color(0, 180, 255));
    m_systemMgr.system<RenderSystem>()->render(target);
    target.draw(m_perfText);

    // DEBUG CODE TO OBSERVE THE GRID INDEXATION
    /*sf::View oldView = target.getView();
    target.setView(m_systemMgr.system<RenderSystem>()->getView());
    m_systemMgr.system<EntityGridSystem>()->getGrid().debugDraw(target, m_gridText);
    target.setView(oldView);*/
    // END OF DEBUG CODE

    auto timeAfter = std::chrono::high_resolution_clock::now();
    m_lastRenderDuration = timeAfter - timeBefore;
    updatePerfText();
}

void LevelState::onError(const std::exception& e)
{
    getStateEngine().stopAndStartState
        <ErrorState, std::string, bool, AllResourcesManagers&, SettingsManager&>(
        std::string(e.what()), false, m_resourcesManager, m_settingsManager
    );
}

void LevelState::receive(const messaging::AllPlayersFinishedMessage& message)
{
    m_asyncExecutor.addNewTask<PunctualTask, std::function<void()>>
    (
        [&]()
        {
            getStateEngine().stopAndStartState
            <LevelSuccessState, AllResourcesManagers&, SettingsManager&>(
                m_resourcesManager, m_settingsManager
            );
        },
        sf::seconds(1.f)
    );
}

void LevelState::receive(const messaging::AllPlayersLostMessage& message)
{
    m_asyncExecutor.addNewTask<PunctualTask, std::function<void()>>
    (
        [&]()
        {
            getStateEngine().stopAndStartState
            <LevelFailureState, const std::string&, AllResourcesManagers&, SettingsManager&>(
                m_path, m_resourcesManager, m_settingsManager
            );
        },
        sf::seconds(1.f)
    );
}

void LevelState::registerClass(LuaState& luaState)
{
    sol::constructors<> ctor;
    sol::usertype<LevelState> levelLuaClass(ctor,
        "create_new_entity", &LevelState::lua_createNewEntity,
        "get_entities", &LevelState::lua_getEntities
    );
    luaState.getState().set_usertype("level_state", levelLuaClass);
}

void LevelState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    auto timeBefore = std::chrono::high_resolution_clock::now();

    m_systemMgr.update<PlayerSystem>(dt.asSeconds());
    m_systemMgr.update<EntityGridSystem>(dt.asSeconds());
    m_systemMgr.update<PlatformerSystem>(dt.asSeconds());
    m_systemMgr.update<CustomBehaviorSystem>(dt.asSeconds());
    m_systemMgr.update<CollisionSystem>(dt.asSeconds());
    m_systemMgr.update<RenderSystem>(dt.asSeconds());
    m_systemMgr.update<HealthSystem>(dt.asSeconds());

    m_asyncExecutor.update(dt);

    auto timeAfter = std::chrono::high_resolution_clock::now();
    m_lastUpdateDuration = timeAfter - timeBefore;
    updatePerfText();

}

EntityHandle LevelState::lua_createNewEntity(const std::string& templateName)
{
    return EntityHandle(m_level->createNewEntity(templateName));
}

std::vector<EntityHandle> LevelState::lua_getEntities(const std::string& templateName)
{
    std::vector<entityx::Entity> entities = m_level->getEntities(templateName);
    std::vector<EntityHandle> handles(entities.begin(), entities.end());
    return handles;
}

void LevelState::updatePerfText()
{
    std::ostringstream os;
    os << "update: " << m_lastUpdateDuration.count() << " ms" << std::endl;
    os << "render: " << m_lastRenderDuration.count() << " ms";

    m_perfText.setString(os.str());
}

}
