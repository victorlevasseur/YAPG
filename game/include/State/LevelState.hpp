#ifndef YAPG_GAME_STATE_LEVELSTATE_H
#define YAPG_GAME_STATE_LEVELSTATE_H

#include <string>

#include "entityx/entityx.h"

#include "Level/Level.hpp"
#include "Lua/LuaState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "State/State.hpp"

namespace state
{

class LevelState : public State
{
public:
    LevelState(const std::string& path, resources::TexturesManager& texturesManager);

    virtual void onStart();

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event);

    virtual void update(sf::Time dt);

    virtual void render(sf::RenderTarget& target);

private:
    lua::LuaState m_luaState;

    level::Level m_level;
    entityx::SystemManager m_systemMgr;
};

}

#endif
