#include "State/LevelEditorState.hpp"

#include "State/StateEngine.hpp"

#include "Systems/RenderSystem.hpp"

namespace state
{

LevelEditorState::LevelEditorState(StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager) :
    State(stateEngine),
    m_luaState(),
    m_level(path, m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager())
{
    m_systemMgr.add<systems::RenderSystem>(resourcesManager.getTextures());

    m_systemMgr.configure();
}

void LevelEditorState::onStop()
{

}

void LevelEditorState::onPause()
{

}

void LevelEditorState::onUnpause()
{

}

void LevelEditorState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelEditorState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 180, 255));
    m_systemMgr.system<systems::RenderSystem>()->render(target);
}

void LevelEditorState::doStart()
{

}

void LevelEditorState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    m_systemMgr.update<systems::RenderSystem>(dt.asSeconds());
}

}
