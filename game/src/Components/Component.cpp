#include "Components/Component.hpp"

#include "Components/CustomBehaviorComponent.hpp"
#include "Components/PositionComponent.hpp"

namespace game
{
namespace components
{

Component::Component()
{

}

Component::~Component()
{

}

void Component::assignComponent(entityx::Entity entity, const std::string &component, const sol::table& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    if(component == "Position")
    {
        entity.assign<PositionComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "CustomBehavior")
    {
        entity.assign<CustomBehaviorComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else
    {
        std::cout << "Unknown component of type \"" << component << "\" !" << std::endl;
    }
}

std::ostream& operator<<(std::ostream& stream, const Component& component)
{
    stream << "Component [" << component.getName() << "]" << std::endl;
    return stream;
}

}
}
