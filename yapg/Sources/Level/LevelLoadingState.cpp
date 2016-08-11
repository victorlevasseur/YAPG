#include "Level/LevelLoadingState.hpp"

#include <algorithm>
#include <cmath>

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
    m_loadedLevel(nullptr),
    m_errored(false)
{
    //Init the loading thread
    auto loadingFunc = [&]() -> void
    {
        try
        {
            m_loadingStatusMutex.lock();
            m_loadingStatusString = "Loading level...";
            m_loadingStatusMutex.unlock();

            LevelLoader levelLoader(m_levelPath);

            //Load the lua state and the level
            m_loadingStatusMutex.lock();
            m_loadingStatusString = "Loading lua...";
            m_loadingStatusMutex.unlock();

            auto luaState = std::make_unique<LuaState>();

            m_loadingStatusMutex.lock();
            m_loadingStatusString = "Loading packages...";
            m_loadingStatusMutex.unlock();

            //Load the dependencies
            std::for_each(
                levelLoader.getDependenciesBegin(),
                levelLoader.getDependenciesEnd(),
                [&](const std::string& packageName)
                {
                    m_loadingStatusMutex.lock();
                    m_loadingStatusString = "Loading " + packageName + "...";
                    m_loadingStatusMutex.unlock();

                    auto packagePromise = PackagesManager::get().getPackage(packageName);
                    packagePromise.get().loadTemplatesIntoLua(*luaState);
                }
            );

            m_loadingStatusMutex.lock();
            m_loadingStatusString = "Creating game entities...";
            m_loadingStatusMutex.unlock();

            auto level = std::make_unique<Level>(*luaState, levelLoader);

            m_loadingStatusMutex.lock();
            m_loadingStatusString = "Starting...";
            m_loadingStatusMutex.unlock();

            m_loadedMutex.lock();

            m_loadedLuaState = std::move(luaState);
            m_loadedLevel = std::move(level);

            m_loadedMutex.unlock();
        }
        catch(...)
        {
            setError("Unknown error!");
        }
    };

    std::thread(loadingFunc).detach(); //Launch the loading thread
}

void LevelLoadingState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    if(m_errored)
    {
        if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Return)
        {
            getStateEngine().stopStateAndUnpause();
        }
    }
}

void LevelLoadingState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));

    if(!m_errored)
        m_loadingText.setColor(sf::Color(255, 255, 255, 127.f + 127.f * std::sin(getTimeSinceStart().asSeconds() * 10.f)));
    else
        m_loadingText.setColor(sf::Color(255, 80, 80, 255));

    m_loadingStatusMutex.lock();
    m_loadingStatusText.setString(m_loadingStatusString);
    m_loadingStatusMutex.unlock();

    //Text positioning
    m_loadingText.setPosition(sf::Vector2f(512.f - m_loadingText.getLocalBounds().width/2.f, 384.f - m_loadingText.getLocalBounds().height/2.f));
    m_loadingStatusText.setPosition(sf::Vector2f(512.f - m_loadingStatusText.getLocalBounds().width/2.f, m_loadingText.getPosition().y + m_loadingText.getLocalBounds().top + m_loadingText.getLocalBounds().height + 32.f));

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

    if(!m_errored && m_loadedLuaState && m_loadedLevel)
    {
        //The level is loaded, launch it.
        getStateEngine().stopAndStartState
        <LevelState, const std::string&, decltype(m_loadedLuaState), decltype(m_loadedLevel), AllResourcesManagers&, SettingsManager&>(
            m_levelPath, std::move(m_loadedLuaState), std::move(m_loadedLevel), m_resourcesManager, m_settingsManager
        );
    }

    m_loadedMutex.unlock();
}

void LevelLoadingState::setError(const std::string& msg)
{
    m_loadingStatusMutex.lock();
    m_loadingText.setString("Loading error :(");
    m_loadingStatusString = msg + "\nPress Enter to return to the main menu";
    m_errored = true;
    m_loadingStatusMutex.unlock();
}

}
