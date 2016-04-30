#include "Level/LevelState.hpp"

#include <sstream>

#include <SFML/System/Time.hpp>

#include "Level/LevelFailureState.hpp"
#include "Level/LevelSuccessState.hpp"
#include "State/MainMenuState.hpp"
#include "State/StateEngine.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/CustomBehaviorSystem.hpp"
#include "Systems/FinishLineSystem.hpp"
#include "Systems/HealthSystem.hpp"
#include "Systems/HitboxUpdaterSystem.hpp"
#include "Systems/PlatformerSystem.hpp"
#include "Systems/PlayerSystem.hpp"
#include "Systems/RenderSystem.hpp"

namespace level
{

LevelState::LevelState(state::StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    state::State(stateEngine),
    m_luaState(),
    m_path(path),
    m_level(m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager()),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_font(resourcesManager.getFonts().requestResource("LiberationSans.ttf")),
    m_perfText("update: -.----\nrender: -.----", *m_font, 14),
    m_lastUpdateDuration(),
    m_lastRenderDuration(),
    m_asyncExecutor()
{
    m_systemMgr.add<systems::HitboxUpdaterSystem>();
    auto& quadtreesGrid = m_systemMgr.system<systems::HitboxUpdaterSystem>()->getQuadTrees();

    m_systemMgr.add<systems::RenderSystem>(resourcesManager.getTextures());
    m_systemMgr.add<systems::CustomBehaviorSystem>();
    m_systemMgr.add<systems::CollisionSystem>(quadtreesGrid);
    m_systemMgr.add<systems::PlatformerSystem>(quadtreesGrid);
    m_systemMgr.add<systems::PlayerSystem>(settingsManager);
    m_systemMgr.add<systems::FinishLineSystem>();
    m_systemMgr.add<systems::HealthSystem>(settingsManager);

    m_systemMgr.configure();

    m_level.LoadFromFile(path);

    m_perfText.setPosition(sf::Vector2f(10.f, 10.f));

    //First update to register the object
    m_systemMgr.update<systems::HitboxUpdaterSystem>(0);
    m_systemMgr.system<systems::HitboxUpdaterSystem>()->getQuadTrees().printContent();
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
            <level::LevelSuccessState, resources::AllResourcesManagers&, settings::SettingsManager&, sfg::SFGUI&, sfg::Desktop&>(
                m_resourcesManager, m_settingsManager, m_sfgui, m_desktop
            );
        },
        sf::seconds(2.f)
    );
}

void LevelState::receive(const messaging::AllPlayersLostMessage& message)
{
    getStateEngine().stopAndStartState
    <level::LevelFailureState, const std::string&, resources::AllResourcesManagers&, settings::SettingsManager&, sfg::SFGUI&, sfg::Desktop&>(
        m_path, m_resourcesManager, m_settingsManager, m_sfgui, m_desktop
    );
}

void LevelState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    auto timeBefore = std::chrono::high_resolution_clock::now();

    m_systemMgr.update<systems::PlayerSystem>(dt.asSeconds());
    m_systemMgr.update<systems::HitboxUpdaterSystem>(dt.asSeconds());
    m_systemMgr.update<systems::PlatformerSystem>(dt.asSeconds());
    m_systemMgr.update<systems::CollisionSystem>(dt.asSeconds());
    m_systemMgr.update<systems::CustomBehaviorSystem>(dt.asSeconds());
    m_systemMgr.update<systems::RenderSystem>(dt.asSeconds());
    m_systemMgr.update<systems::FinishLineSystem>(dt.asSeconds());
    m_systemMgr.update<systems::HealthSystem>(dt.asSeconds());

    m_asyncExecutor.update(dt);

    auto timeAfter = std::chrono::high_resolution_clock::now();
    m_lastUpdateDuration = timeAfter - timeBefore;
    updatePerfText();

}

void LevelState::updatePerfText()
{
    std::ostringstream os;
    os << "update: " << m_lastUpdateDuration.count() << " ms" << std::endl;
    os << "render: " << m_lastRenderDuration.count() << " ms";

    m_perfText.setString(os.str());
}

}
