#ifndef YAPG_GAME_ERROR_ERRORSTATE_H
#define YAPG_GAME_ERROR_ERRORSTATE_H

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

class ErrorState : public State
{
public:
    ErrorState(StateEngine& stateEngine, const std::string& errorMsg, bool fatal, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doStop();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget& target);

private:
    AllResourcesManagers& m_resourcesManager;
    SettingsManager& m_settingsManager;

    std::shared_ptr<sf::Font> m_font;
    sf::Text m_errorText;
    sf::Text m_errorDetailText;
    std::string m_errorMsg;
    bool m_fatal;
};

}

#endif
