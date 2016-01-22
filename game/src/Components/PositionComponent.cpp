#include "Components/PositionComponent.hpp"

namespace game
{
namespace components
{

PositionComponent::PositionComponent() :
    Component(),
    x(0.0),
    y(0.0),
    width(0.0),
    height(0.0)
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
    x = luaTable.get<float>("x");
    y = luaTable.get<float>("y");
    width = luaTable.get<float>("width");
    height = luaTable.get<float>("height");
}

void PositionComponent::registerComponent(lua::LuaState& state)
{
    state.getState().new_usertype<PositionComponent>("position_component",
        "x", &PositionComponent::x,
        "y", &PositionComponent::y,
        "width", &PositionComponent::width,
        "height", &PositionComponent::height
    );
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
