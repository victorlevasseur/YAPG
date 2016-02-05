#include "Lua/EntityHandle.hpp"

#include <iostream>

#include "Components/Component.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Lua/LuaState.hpp"

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

components::Component* EntityHandle::getComponent(const std::string& componentName)
{
    if(componentName == "Position")
        return m_entity.component<components::PositionComponent>().get();
    else if(componentName == "Render")
        return m_entity.component<components::RenderComponent>().get();
}

void EntityHandle::writeToConsole(const std::string& str)
{
    std::cout << str << std::endl;
}

void EntityHandle::removeEntity()
{
    m_entity.destroy();
}

void EntityHandle::registerClass(LuaState &state)
{
    state.getState().new_usertype<EntityHandle>("entity_handle",
        "remove_entity", &EntityHandle::removeEntity,
        "get_component", &EntityHandle::getComponent,
        "write_to_console", &EntityHandle::writeToConsole
    );
}

}
