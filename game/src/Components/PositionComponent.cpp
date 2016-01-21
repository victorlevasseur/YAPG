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

void PositionComponent::loadFromLua(const sol::table& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    x = luaTable.get<int>("x");
    y = luaTable.get<int>("y");
    width = luaTable.get<int>("width");
    height = luaTable.get<int>("height");
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
