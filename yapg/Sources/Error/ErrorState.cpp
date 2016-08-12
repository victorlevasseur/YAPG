#include "Error/ErrorState.hpp"

#include <algorithm>
#include <cmath>
#include <exception>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Gui/imgui.h"

#include "Level/Level.hpp"
#include "Level/LevelState.hpp"
#include "Level/Serialization/LevelLoader.hpp"
#include "Lua/LuaState.hpp"
#include "State/StateEngine.hpp"
#include "Template/PackagesManager.hpp"

namespace yapg
{

ErrorState::ErrorState(StateEngine& stateEngine, const std::string& errorMsg, bool fatal, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager) :
    State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_font(m_resourcesManager.getFonts().requestResource("assets/LiberationSans.ttf")),
    m_errorText("Something went wrong :(!", *m_font, 70u),
    m_errorDetailText(errorMsg + "\nPress Enter to " + (fatal ? "quit" : "return to the main menu"), *m_font, 12u),
    m_errorMsg(errorMsg),
    m_fatal(fatal)
{
    m_errorText.setColor(sf::Color(255, 80, 80, 255));
}

void ErrorState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
    {
        if(m_fatal)
        {
            std::cout << "[Fatal error] YAPG terminated because of " << m_errorMsg << std::endl;
            std::terminate();
        }
        else
            getStateEngine().stopStateAndUnpause();
    }
}

void ErrorState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));

    //Text positioning
    m_errorText.setPosition(sf::Vector2f(512.f - m_errorText.getLocalBounds().width/2.f, 384.f - m_errorText.getLocalBounds().height/2.f));
    m_errorDetailText.setPosition(sf::Vector2f(512.f - m_errorDetailText.getLocalBounds().width/2.f, m_errorText.getPosition().y + m_errorText.getLocalBounds().top + m_errorText.getLocalBounds().height + 32.f));

    target.draw(m_errorText);
    target.draw(m_errorDetailText);
}

void ErrorState::doStart()
{

}

void ErrorState::doStop()
{

}

void ErrorState::doUpdate(sf::Time dt, sf::RenderTarget& target)
{

}

}
