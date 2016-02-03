#ifndef YAPG_GAME_LEVEL_LEVEL_H
#define YAPG_GAME_LEVEL_LEVEL_H

#include "entityx/entityx.h"

#include "Level/SerializedEntityGetter.hpp"
#include "Lua/LuaState.hpp"


namespace level
{

class Level
{
public:
    Level(const std::string& path, lua::LuaState& luaState);

    const entityx::EventManager& getEventManager() const { return m_eventMgr; }
    entityx::EventManager& getEventManager() { return m_eventMgr; }

    const entityx::EntityManager& getEntityManager() const { return m_entityMgr; }
    entityx::EntityManager& getEntityManager() { return m_entityMgr; }

private:
    lua::LuaState& m_luaState;

    entityx::EventManager m_eventMgr;
    entityx::EntityManager m_entityMgr;
};

}

#endif
