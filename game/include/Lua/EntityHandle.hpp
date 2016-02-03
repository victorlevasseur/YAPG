#ifndef YAPG_GAME_LUA_ENTITYHANDLE_H
#define YAPG_GAME_LUA_ENTITYHANDLE_H

#include "entityx/entityx.h"

#include "Lua/sol.hpp"

namespace components{ class PositionComponent; }
namespace lua{ class LuaState; }


namespace lua
{

class EntityHandle
{
public:
    EntityHandle();
    EntityHandle(entityx::Entity entity);

    components::PositionComponent* getPositionComponent();

    void writeToConsole(const std::string& str);

    void removeEntity();

    static void registerClass(LuaState &state);

private:
    entityx::Entity m_entity;
};

}

#endif
