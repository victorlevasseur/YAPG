#ifndef YAPG_GAME_STATE_LEVELSTATE_H
#define YAPG_GAME_STATE_LEVELSTATE_H

#include <string>

#include "entityx/entityx.h"

#include <SFGUI/Desktop.hpp>
#include <SFGUI/SFGUI.hpp>

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
    LevelState(state::StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop);

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
    sfg::SFGUI& m_sfgui;
    sfg::Desktop& m_desktop;

    async::AsyncExecutor m_asyncExecutor;
};

}

#endif
