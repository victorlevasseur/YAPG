#ifndef YAPG_GAME_STATE_LEVELSTATE_H
#define YAPG_GAME_STATE_LEVELSTATE_H

#include <chrono>
#include <memory>
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

namespace yapg{ class StateEngine; }

namespace yapg
{

class LevelState : public State, public messaging::Emitter, public messaging::Receiver<messaging::AllPlayersFinishedMessage, messaging::AllPlayersLostMessage>
{
public:
    LevelState(StateEngine& stateEngine, std::string path, std::unique_ptr<LuaState>&& luaState, std::unique_ptr<Level>&& level, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

    virtual void onError(const std::exception& e);

    virtual void receive(const messaging::AllPlayersFinishedMessage& message) override;
    virtual void receive(const messaging::AllPlayersLostMessage& message) override;

    static void registerClass(LuaState& luaState);

protected:
    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    EntityHandle lua_createNewEntity(const std::string& templateName);

    std::string m_path;
    std::unique_ptr<LuaState> m_luaState;
    std::unique_ptr<Level> m_level;
    entityx::SystemManager m_systemMgr;

    AllResourcesManagers& m_resourcesManager;
    SettingsManager& m_settingsManager;

    void updatePerfText();

    std::shared_ptr<sf::Font> m_font;
    sf::Text m_perfText;
    std::chrono::duration<double, std::milli> m_lastUpdateDuration;
    std::chrono::duration<double, std::milli> m_lastRenderDuration;

    sf::Text m_gridText;

    AsyncExecutor m_asyncExecutor;
};

}

#endif
