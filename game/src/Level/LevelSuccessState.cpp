#include "Level/LevelSuccessState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include "imgui.h"

#include "State/StateEngine.hpp"

namespace level
{

LevelSuccessState::LevelSuccessState(state::StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager) :
    state::State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_winMusic(m_resourcesManager.getSounds().requestResource("menu/bensound-happiness.ogg")),
    m_winSound(*m_winMusic),
    m_font(m_resourcesManager.getFonts().requestResource("LiberationSans.ttf")),
    m_winText("Level clear !", *m_font, 70u)
{
    //Sound init
    m_winSound.setLoop(true);
    m_winSound.setVolume(20.f);

    //Text init
    m_winText.setPosition(sf::Vector2f(512.f - m_winText.getLocalBounds().width/2.f, 200.f));
}

void LevelSuccessState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelSuccessState::render(sf::RenderTarget& target)
{
    //GUI
    ImGui::SetNextWindowPosCenter();
    ImGui::Begin("Menu");
        if(ImGui::Button("Return to main menu"))
        {
            getStateEngine().stopStateAndUnpause();
        }
    ImGui::End();

    target.clear(sf::Color(80, 255, 80));

    target.draw(m_winText);
}

void LevelSuccessState::doStart()
{
    m_winSound.play();
}

void LevelSuccessState::doStop()
{
    m_winSound.stop();
}

void LevelSuccessState::doUpdate(sf::Time dt, sf::RenderTarget& target)
{

}

}
