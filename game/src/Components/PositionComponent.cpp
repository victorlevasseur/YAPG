#include "Components/PositionComponent.hpp"

#include "Lua/Loader.hpp"

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

void PositionComponent::loadFromLua(const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    lua::MetadataStore::getMetadata<PositionComponent>().load(this, luaTable);
}

void PositionComponent::registerComponent(lua::LuaState& state)
{
    //Register loading infos
    lua::MetadataStore::registerClass<PositionComponent>()
        .declareLoadableAttribute<float>("x", &PositionComponent::x)
        .declareLoadableAttribute<float>("y", &PositionComponent::y)
        .declareLoadableAttribute<float>("width", &PositionComponent::width)
        .declareLoadableAttribute<float>("height", &PositionComponent::height);

    //Register to lua
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
