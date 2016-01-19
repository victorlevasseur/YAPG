#include "Components/Component.hpp"

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

void Component::assignComponent(entityx::Entity entity, const std::string &component, sel::Selector& luaSelector)
{
    if(component == "Position")
    {
        entity.assign<PositionComponent>().get()->loadFromLua(luaSelector);
    }
}

std::ostream& operator<<(std::ostream& stream, const Component& component)
{
    stream << "Component [" << component.getName() << "]" << std::endl;
    return stream;
}

}
}
