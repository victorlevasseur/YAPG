#include "Level/LevelFailureState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "Gui/imgui.h"

#include "Level/LevelState.hpp"
#include "State/StateEngine.hpp"

namespace yapg
{

LevelFailureState::LevelFailureState(StateEngine& stateEngine, const std::string& levelPath, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager) :
    State(stateEngine),
    m_levelPath(levelPath),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_looseMusic(m_resourcesManager.getSounds().requestResource("menu/bensound-sadday.ogg")),
    m_looseSound(*m_looseMusic),
    m_font(m_resourcesManager.getFonts().requestResource("LiberationSans.ttf")),
    m_looseText("You failed !", *m_font, 70u)
{
    //Sound init
    m_looseSound.setLoop(true);
    m_looseSound.setVolume(20.f);

    //Text init
    m_looseText.setPosition(sf::Vector2f(512.f - m_looseText.getLocalBounds().width/2.f, 200.f));
}

void LevelFailureState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelFailureState::render(sf::RenderTarget& target)
{
    //GUI
    ImGui::SetNextWindowSize(ImVec2(220.f, 100.f));
    ImGui::SetNextWindowPosCenter();
    ImGui::Begin("Menu");
        if(ImGui::Button("Try again"))
        {
            getStateEngine().stopAndStartState
            <LevelState, const std::string&, AllResourcesManagers&, SettingsManager&>(
                m_levelPath, m_resourcesManager, m_settingsManager
            );
        }
        if(ImGui::Button("Return to main menu"))
        {
            getStateEngine().stopStateAndUnpause();
        }
    ImGui::End();

    target.clear(sf::Color(255, 80, 80));

    target.draw(m_looseText);
}

void LevelFailureState::doStart()
{
    m_looseSound.play();
}

void LevelFailureState::doStop()
{
    m_looseSound.stop();
}

void LevelFailureState::doUpdate(sf::Time dt, sf::RenderTarget& target)
{

}

}
