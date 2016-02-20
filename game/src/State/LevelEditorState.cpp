#include "State/LevelEditorState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "State/StateEngine.hpp"
#include "Systems/RenderSystem.hpp"

namespace state
{

LevelEditorState::LevelEditorState(StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    State(stateEngine),
    m_luaState(),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_level(path, m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager())
{
    m_systemMgr.add<systems::RenderSystem>(resourcesManager.getTextures());

    m_desktop.Add(sfg::Window::Create());

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
    m_desktop.HandleEvent(event);
}

void LevelEditorState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 180, 255));
    m_systemMgr.system<systems::RenderSystem>()->render(target);
    m_sfgui.Display(dynamic_cast<sf::RenderWindow&>(target));
}

void LevelEditorState::doStart()
{

}

void LevelEditorState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    m_systemMgr.update<systems::RenderSystem>(dt.asSeconds());
    m_desktop.Update(dt.asSeconds());
}

}
