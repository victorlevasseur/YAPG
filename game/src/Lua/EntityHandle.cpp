#include "Lua/EntityHandle.hpp"

#include "Components/PositionComponent.hpp"
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

components::PositionComponent* EntityHandle::getPositionComponent()
{
    if(m_entity.has_component<components::PositionComponent>())
        return m_entity.component<components::PositionComponent>().get();

    return nullptr;
}

void EntityHandle::removeEntity()
{
    m_entity.destroy();
}

void EntityHandle::registerClass(LuaState &state)
{
    state.getState().new_usertype<EntityHandle>("entity_handle",
        "remove_entity", &EntityHandle::removeEntity,
        "position_component", &EntityHandle::getPositionComponent
    );
}

}
}
