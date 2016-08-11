#ifndef YAPG_GAME_LEVEL_LEVELLOADINGSTATE_H
#define YAPG_GAME_LEVEL_LEVELLOADINGSTATE_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Async/AsyncExecutor.hpp"
#include "Level/Level.hpp"
#include "Lua/LuaState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace yapg{ class StateEngine; }

namespace yapg
{

class LevelLoadingState : public State
{
public:
    LevelLoadingState(StateEngine& stateEngine, const std::string& levelPath, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doStop();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget& target);

private:
    void setError(const std::string& msg);

    std::string m_levelPath;

    AllResourcesManagers& m_resourcesManager;
    SettingsManager& m_settingsManager;

    std::shared_ptr<sf::Font> m_font;
    sf::Text m_loadingText;
    sf::Text m_loadingStatusText;

    std::mutex m_loadingStatusMutex;
    std::string m_loadingStatusString;

    std::mutex m_loadedMutex;
    std::unique_ptr<LuaState> m_loadedLuaState;
    std::unique_ptr<Level> m_loadedLevel;
    std::atomic<bool> m_errored;
};

}

#endif
