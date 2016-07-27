#include "Level/LevelLoadingState.hpp"

#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>

#include "Gui/imgui.h"

#include "Level/Level.hpp"
#include "Level/LevelState.hpp"
#include "Lua/LuaState.hpp"
#include "State/StateEngine.hpp"

namespace yapg
{

LevelLoadingState::LevelLoadingState(StateEngine& stateEngine, const std::string& levelPath, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager) :
    State(stateEngine),
    m_levelPath(levelPath),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_font(m_resourcesManager.getFonts().requestResource("assets/LiberationSans.ttf")),
    m_loadingText("Loading...", *m_font, 70u),
    m_loadingStatusText("", *m_font, 40u),
    m_loadingStatusMutex(),
    m_loadingStatusString(),
    m_loadedMutex(),
    m_loadedLuaState(nullptr),
    m_loadedLevel(nullptr)
{
    //Text init
    m_loadingText.setPosition(sf::Vector2f(512.f - m_loadingText.getLocalBounds().width/2.f, 384.f - m_loadingText.getLocalBounds().height/2.f));

    //Init the loading thread
    auto loadingFunc = [&]() -> void
    {
        //Load the lua state and the level
        m_loadingStatusMutex.lock();
        m_loadingStatusString = "Loading lua...";
        m_loadingStatusMutex.unlock();

        auto luaState = std::make_unique<LuaState>();

        m_loadingStatusMutex.lock();
        m_loadingStatusString = "Loading level...";
        m_loadingStatusMutex.unlock();

        auto level = std::make_unique<Level>(*luaState);
        level->loadFromFile(m_levelPath);

        m_loadingStatusMutex.lock();
        m_loadingStatusString = "Starting...";
        m_loadingStatusMutex.unlock();

        m_loadedMutex.lock();

        m_loadedLuaState = std::move(luaState);
        m_loadedLevel = std::move(level);

        m_loadedMutex.unlock();
    };

    std::thread(loadingFunc).detach(); //Launch the loading thread
}

void LevelLoadingState::processEvent(sf::Event event, sf::RenderTarget &target)
{

}

void LevelLoadingState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));

    m_loadingText.setColor(sf::Color(255, 255, 255, 127.f + 127.f * std::sin(getTimeSinceStart().asSeconds() * 10.f)));

    m_loadingStatusMutex.lock();
    m_loadingStatusText.setString(m_loadingStatusString);
    m_loadingStatusMutex.unlock();
    m_loadingStatusText.setPosition(sf::Vector2f(512.f - m_loadingStatusText.getLocalBounds().width/2.f, 454.f - m_loadingStatusText.getLocalBounds().height/2.f));

    target.draw(m_loadingText);
    target.draw(m_loadingStatusText);
}

void LevelLoadingState::doStart()
{

}

void LevelLoadingState::doStop()
{

}

void LevelLoadingState::doUpdate(sf::Time dt, sf::RenderTarget& target)
{
    //Test if the level has been loaded.
    m_loadedMutex.lock();

    if(m_loadedLuaState && m_loadedLevel)
    {
        //The level is loaded, launch it.
        getStateEngine().stopAndStartState
        <LevelState, const std::string&, decltype(m_loadedLuaState), decltype(m_loadedLevel), AllResourcesManagers&, SettingsManager&>(
            m_levelPath, std::move(m_loadedLuaState), std::move(m_loadedLevel), m_resourcesManager, m_settingsManager
        );
    }

    m_loadedMutex.unlock();
}

}
