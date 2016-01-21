#include "Lua/EntityHandle.hpp"

#include "Lua/LuaState.hpp"

namespace game
{
namespace lua
{

EntityHandle::EntityHandle() :
    m_entity()
{

}

EntityHandle::EntityHandle(entityx::Entity entity) :
    m_entity(entity)
{

}

void EntityHandle::removeEntity()
{
    m_entity.destroy();
}

void EntityHandle::RegisterClass(LuaState &state)
{
    state.getState().new_usertype<EntityHandle>("entity_handle",
        "remove_entity", &EntityHandle::removeEntity
    );
}

}
}
