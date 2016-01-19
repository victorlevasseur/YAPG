#ifndef YAPG_GAME_LUA_ENTITYHANDLE_H
#define YAPG_GAME_LUA_ENTITYHANDLE_H

#include "entityx/entityx.h"

namespace game{ namespace lua{ class LuaState; } }

namespace game
{
namespace lua
{

class EntityHandle
{
public:
    EntityHandle();
    EntityHandle(entityx::Entity entity);

    void removeEntity();

    static void RegisterClass(LuaState &state);

private:
    entityx::Entity m_entity;
};

}
}

#endif
