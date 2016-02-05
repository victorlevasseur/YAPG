#ifndef YAPG_GAME_LUA_ENTITYHANDLE_H
#define YAPG_GAME_LUA_ENTITYHANDLE_H

#include "entityx/entityx.h"

#include "Lua/sol.hpp"

namespace components{ class Component; }
namespace lua{ class LuaState; }


namespace lua
{

class EntityHandle
{
public:
    EntityHandle();
    EntityHandle(entityx::Entity entity);

    components::Component* getComponent(const std::string& componentName);

    void writeToConsole(const std::string& str);

    void removeEntity();

    static void registerClass(LuaState &state);

private:
    entityx::Entity m_entity;
};

}

#endif
