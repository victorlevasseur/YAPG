#include "State/LevelState.hpp"

#include "Systems/CustomBehaviorSystem.hpp"
#include "Systems/HitboxUpdaterSystem.hpp"
#include "Systems/PlatformerSystem.hpp"
#include "Systems/RenderSystem.hpp"


namespace state
{

LevelState::LevelState(const std::string& path, resources::TexturesManager& texturesManager, settings::SettingsManager& settingsManager) :
    State(),
    m_luaState(),
    m_level(path, m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager())
{
    m_systemMgr.add<systems::RenderSystem>(texturesManager);
    m_systemMgr.add<systems::CustomBehaviorSystem>();
    m_systemMgr.add<systems::PlatformerSystem>();
    m_systemMgr.add<systems::HitboxUpdaterSystem>();

    m_systemMgr.configure();
}

void LevelState::onStart()
{

}

void LevelState::onStop()
{

}

void LevelState::onPause()
{

}

void LevelState::onUnpause()
{

}

void LevelState::processEvent(sf::Event event)
{

}

void LevelState::update(sf::Time dt)
{
    m_systemMgr.update<systems::HitboxUpdaterSystem>(dt.asSeconds());
    m_systemMgr.update<systems::PlatformerSystem>(dt.asSeconds());
    m_systemMgr.update<systems::CustomBehaviorSystem>(dt.asSeconds());
    m_systemMgr.update<systems::RenderSystem>(dt.asSeconds());
}

void LevelState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 180, 255));
    m_systemMgr.system<systems::RenderSystem>()->render(target);
}

}
