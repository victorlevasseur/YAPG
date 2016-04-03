#include "Level/LevelState.hpp"

#include <SFML/System/Time.hpp>

#include "Level/LevelSuccessState.hpp"
#include "State/MainMenuState.hpp"
#include "State/StateEngine.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/CustomBehaviorSystem.hpp"
#include "Systems/FinishLineSystem.hpp"
#include "Systems/HitboxUpdaterSystem.hpp"
#include "Systems/PlatformerSystem.hpp"
#include "Systems/PlayerSystem.hpp"
#include "Systems/RenderSystem.hpp"

namespace level
{

LevelState::LevelState(state::StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    state::State(stateEngine),
    m_luaState(),
    m_level(m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager()),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_asyncExecutor()
{
    m_systemMgr.add<systems::RenderSystem>(resourcesManager.getTextures());
    m_systemMgr.add<systems::CustomBehaviorSystem>();
    m_systemMgr.add<systems::CollisionSystem>();
    m_systemMgr.add<systems::PlatformerSystem>();
    m_systemMgr.add<systems::HitboxUpdaterSystem>();
    m_systemMgr.add<systems::PlayerSystem>(settingsManager);
    m_systemMgr.add<systems::FinishLineSystem>();

    m_systemMgr.configure();

    m_level.LoadFromFile(path);
}

void LevelState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 180, 255));
    m_systemMgr.system<systems::RenderSystem>()->render(target);
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

void LevelState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    m_systemMgr.update<systems::PlayerSystem>(dt.asSeconds());
    m_systemMgr.update<systems::HitboxUpdaterSystem>(dt.asSeconds());
    m_systemMgr.update<systems::PlatformerSystem>(dt.asSeconds());
    m_systemMgr.update<systems::CollisionSystem>(dt.asSeconds());
    m_systemMgr.update<systems::CustomBehaviorSystem>(dt.asSeconds());
    m_systemMgr.update<systems::RenderSystem>(dt.asSeconds());
    m_systemMgr.update<systems::FinishLineSystem>(dt.asSeconds());

    m_asyncExecutor.update(dt);
}

}
