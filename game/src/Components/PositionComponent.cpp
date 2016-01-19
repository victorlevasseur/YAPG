#include "Components/PositionComponent.hpp"

namespace game
{
namespace components
{

PositionComponent::PositionComponent() :
    x(0),
    y(0),
    width(0),
    height(0)
{

}

PositionComponent::~PositionComponent()
{

}

std::string PositionComponent::getName() const
{
    return "Position";
}

void PositionComponent::loadFromLua(sel::Selector& luaSelector)
{
    x = (int)luaSelector["x"];
    y = (int)luaSelector["y"];
    width = (int)luaSelector["width"];
    height = (int)luaSelector["height"];
}

std::ostream& operator<<(std::ostream& stream, const PositionComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    stream << "x=" << component.x << ", y=" << component.y << ", width=" <<
        component.width << ", height=" << component.height << std::endl;
    return stream;
}

}
}
