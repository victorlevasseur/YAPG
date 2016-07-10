#include "Level/LevelState.hpp"

#include <sstream>

#include <SFML/System/Time.hpp>

#include "Components/PlatformerHitboxComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"

#include "Level/LevelFailureState.hpp"
#include "Level/LevelSuccessState.hpp"
#include "State/MainMenuState.hpp"
#include "State/StateEngine.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/CustomBehaviorSystem.hpp"
#include "Systems/EntityGridSystem.hpp"
#include "Systems/FinishLineSystem.hpp"
#include "Systems/HealthSystem.hpp"
#include "Systems/PlatformerSystem.hpp"
#include "Systems/PlayerSystem.hpp"
#include "Systems/RenderSystem.hpp"

namespace level
{

LevelState::LevelState(state::StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager) :
    state::State(stateEngine),
    m_luaState(),
    m_path(path),
    m_level(m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager()),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_font(resourcesManager.getFonts().requestResource("LiberationSans.ttf")),
    m_perfText("update: -.----\nrender: -.----", *m_font, 14),
    m_lastUpdateDuration(),
    m_lastRenderDuration(),
    m_gridText("", *m_font, 16),
    m_asyncExecutor()
{
    m_systemMgr.add<systems::EntityGridSystem>();
    auto& grid = m_systemMgr.system<systems::EntityGridSystem>()->getGrid();

    m_systemMgr.add<systems::RenderSystem>(resourcesManager.getTextures(), grid);
    m_systemMgr.add<systems::CustomBehaviorSystem>();
    m_systemMgr.add<systems::CollisionSystem>(grid);
    m_systemMgr.add<systems::PlatformerSystem>(grid);
    m_systemMgr.add<systems::PlayerSystem>(settingsManager);
    m_systemMgr.add<systems::HealthSystem>(settingsManager);

    m_systemMgr.configure();

    m_level.LoadFromFile(path);

    //Load the players
    //TODO: Support multiple players creation
    m_stillPlayingCount = m_playersCount = 1;
    std::cout << "Creating players..." << std::endl;

    const lua::EntityTemplate& playerTemplate = m_luaState.getTemplate(m_level.getPlayersTemplates()[0]);

    entityx::Entity playerEntity = m_level.getEntityManager().create();
    playerTemplate.initializeEntity(playerEntity, SerializedEntityGetter());

    //Set x and y parameters according to spawn_position
    auto& parameters = playerTemplate.getParameters();

    const lua::EntityTemplate::Parameter& xParameter = parameters.at("x");
    const lua::EntityTemplate::Parameter& yParameter = parameters.at("y");

    lua::EntityHandle(playerEntity).setAttributeAsAny(xParameter.component, xParameter.attribute, m_level.getSpawnPosition().x);
    lua::EntityHandle(playerEntity).setAttributeAsAny(yParameter.component, yParameter.attribute, m_level.getSpawnPosition().y);

    if(!playerEntity.has_component<components::PlayerComponent>())
        throw std::runtime_error(std::string("[Level/Error] Player entities must have the \"Player\" component declared in their template ! Not the case with \"") + m_level.getPlayersTemplates()[0] + std::string("\""));
    playerEntity.component<components::PlayerComponent>()->playerNumber = 0;

    std::cout << "Players created." << std::endl;
    /////////////////////////////////////////

    //Debug info
    m_perfText.setPosition(sf::Vector2f(10.f, 10.f));
    m_perfText.setColor(sf::Color::Black);
    m_gridText.setColor(sf::Color::Black);

    //First update to register the object
    m_systemMgr.update<systems::EntityGridSystem>(0);

    //Put the current level instance into "current_level" lua global variable
    m_luaState.getState().set("current_level", this);
}

void LevelState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelState::render(sf::RenderTarget& target)
{
    auto timeBefore = std::chrono::high_resolution_clock::now();

    target.clear(sf::Color(0, 180, 255));
    m_systemMgr.system<systems::RenderSystem>()->render(target);
    target.draw(m_perfText);

    // DEBUG CODE TO OBSERVE THE GRID INDEXATION
    /*sf::View oldView = target.getView();
    target.setView(m_systemMgr.system<systems::RenderSystem>()->getView());
    m_systemMgr.system<systems::EntityGridSystem>()->getGrid().debugDraw(target, m_gridText);
    target.setView(oldView);*/
    // END OF DEBUG CODE

    auto timeAfter = std::chrono::high_resolution_clock::now();
    m_lastRenderDuration = timeAfter - timeBefore;
    updatePerfText();
}

void LevelState::receive(const messaging::AllPlayersFinishedMessage& message)
{
    m_asyncExecutor.addNewTask<async::PunctualTask, std::function<void()>>
    (
        [&]()
        {
            getStateEngine().stopAndStartState
            <level::LevelSuccessState, resources::AllResourcesManagers&, settings::SettingsManager&>(
                m_resourcesManager, m_settingsManager
            );
        },
        sf::seconds(1.f)
    );
}

void LevelState::receive(const messaging::AllPlayersLostMessage& message)
{
    getStateEngine().stopAndStartState
    <level::LevelFailureState, const std::string&, resources::AllResourcesManagers&, settings::SettingsManager&>(
        m_path, m_resourcesManager, m_settingsManager
    );
}

void LevelState::registerClass(lua::LuaState& luaState)
{
    sol::constructors<> ctor;
    sol::usertype<LevelState> levelLuaClass(ctor,
        "create_new_entity", &LevelState::lua_createNewEntity,
        "set_player_finished", &LevelState::lua_setPlayerFinished
    );
    luaState.getState().set_usertype("level_state", levelLuaClass);
}

void LevelState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    auto timeBefore = std::chrono::high_resolution_clock::now();

    m_systemMgr.update<systems::PlayerSystem>(dt.asSeconds());
    m_systemMgr.update<systems::EntityGridSystem>(dt.asSeconds());
    m_systemMgr.update<systems::PlatformerSystem>(dt.asSeconds());
    m_systemMgr.update<systems::CustomBehaviorSystem>(dt.asSeconds());
    m_systemMgr.update<systems::CollisionSystem>(dt.asSeconds());
    m_systemMgr.update<systems::RenderSystem>(dt.asSeconds());
    m_systemMgr.update<systems::HealthSystem>(dt.asSeconds());

    m_asyncExecutor.update(dt);

    auto timeAfter = std::chrono::high_resolution_clock::now();
    m_lastUpdateDuration = timeAfter - timeBefore;
    updatePerfText();

}

lua::EntityHandle LevelState::lua_createNewEntity(const std::string& templateName)
{
    return lua::EntityHandle(m_level.createNewEntity(templateName));
}

void LevelState::lua_setPlayerFinished(lua::EntityHandle playerEntity_)
{
    entityx::Entity playerEntity = static_cast<entityx::Entity>(playerEntity_);
    if(!playerEntity.has_component<components::PlayerComponent>())
    {
        std::cout << "[Lua/Warning] The entity given to set_player_finished is not a player !" << std::endl;
        return;
    }

    auto playerComponent = playerEntity.component<components::PlayerComponent>();

    if(!playerComponent->finishedLevel)
    {
        playerComponent->finishedLevel = true;
        --m_stillPlayingCount;
        emit<messaging::PlayerFinishedMessage>(playerComponent->playerNumber);

        if(m_stillPlayingCount == 0)
        {
            emit<messaging::AllPlayersFinishedMessage>();
        }
    }
}

void LevelState::updatePerfText()
{
    std::ostringstream os;
    os << "update: " << m_lastUpdateDuration.count() << " ms" << std::endl;
    os << "render: " << m_lastRenderDuration.count() << " ms";

    m_perfText.setString(os.str());
}

}
