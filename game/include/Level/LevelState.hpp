#ifndef YAPG_GAME_STATE_LEVELSTATE_H
#define YAPG_GAME_STATE_LEVELSTATE_H

#include <chrono>
#include <string>

#include "entityx/entityx.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Async/AsyncExecutor.hpp"
#include "Async/PunctualTask.hpp"
#include "Level/Level.hpp"
#include "Lua/LuaState.hpp"
#include "Messaging/LevelMessages.hpp"
#include "Messaging/Messaging.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace level
{

class LevelState : public state::State, public messaging::Receiver<messaging::AllPlayersFinishedMessage, messaging::AllPlayersLostMessage>
{
public:
    LevelState(state::StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

    virtual void receive(const messaging::AllPlayersFinishedMessage& message) override;
    virtual void receive(const messaging::AllPlayersLostMessage& message) override;

protected:
    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    lua::LuaState m_luaState;

    std::string m_path;
    level::Level m_level;
    entityx::SystemManager m_systemMgr;

    resources::AllResourcesManagers& m_resourcesManager;
    settings::SettingsManager& m_settingsManager;

    void updatePerfText();

    std::shared_ptr<sf::Font> m_font;
    sf::Text m_perfText;
    std::chrono::duration<double, std::milli> m_lastUpdateDuration;
    std::chrono::duration<double, std::milli> m_lastRenderDuration;

    sf::Text m_gridText;

    async::AsyncExecutor m_asyncExecutor;
};

}

#endif
