#ifndef YAPG_GAME_STATE_LEVELSTATE_H
#define YAPG_GAME_STATE_LEVELSTATE_H

#include <string>

#include "entityx/entityx.h"

#include "Level/Level.hpp"
#include "Lua/LuaState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace state
{

class LevelState : public State
{
public:
    LevelState(StateEngine& stateEngine, std::string path, resources::TexturesManager& texturesManager, settings::SettingsManager& settingsManager);

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doUpdate(sf::Time dt);

private:
    lua::LuaState m_luaState;

    level::Level m_level;
    entityx::SystemManager m_systemMgr;
};

}

#endif
