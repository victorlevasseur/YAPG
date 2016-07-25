#include "Level/LevelLoadingState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Gui/imgui.h"

#include "Level/LevelState.hpp"
#include "State/StateEngine.hpp"

namespace yapg
{

LevelLoadingState::LevelLoadingState(StateEngine& stateEngine, const std::string& levelPath, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager) :
    State(stateEngine),
    m_levelPath(levelPath),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_font(m_resourcesManager.getFonts().requestResource("assets/LiberationSans.ttf")),
    m_loadingText("Loading...", *m_font, 70u)
{
    //Text init
    m_loadingText.setPosition(sf::Vector2f(512.f - m_loadingText.getLocalBounds().width/2.f, 384.f - m_loadingText.getLocalBounds().height/2.f));
}

void LevelLoadingState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelLoadingState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));

    target.draw(m_loadingText);

    getStateEngine().stopAndStartState
    <LevelState, std::string, AllResourcesManagers&, SettingsManager&>(
        std::string(m_levelPath), m_resourcesManager, m_settingsManager
    );
}

void LevelLoadingState::doStart()
{

}

void LevelLoadingState::doStop()
{

}

void LevelLoadingState::doUpdate(sf::Time dt, sf::RenderTarget& target)
{

}

}
