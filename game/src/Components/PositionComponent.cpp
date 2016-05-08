#include "Components/PositionComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

PositionComponent::PositionComponent(entityx::Entity entity) :
    Component(entity),
    x(0.f),
    y(0.f),
    z(0.f),
    width(0.f),
    height(0.f),
    oldX(std::numeric_limits<float>::lowest()),
    oldY(std::numeric_limits<float>::lowest()),
    oldWidth(std::numeric_limits<float>::lowest()),
    oldHeight(std::numeric_limits<float>::lowest())
{

}

PositionComponent::~PositionComponent()
{

}

std::string PositionComponent::getName() const
{
    return "Position";
}

void PositionComponent::registerComponent(lua::LuaState& state)
{
    //Register loading infos
    meta::MetadataStore::registerClass<PositionComponent>("PositionComponent")
        .declareAttribute<float>("x", &PositionComponent::x)
        .declareAttribute<float>("y", &PositionComponent::y)
        .declareAttribute<float>("z", &PositionComponent::z)
        .declareAttribute<float>("width", &PositionComponent::width)
        .declareAttribute<float>("height", &PositionComponent::height);

    lua::EntityHandle::declareComponent<PositionComponent>("position");

    //Register to lua
    state.getState().new_usertype<PositionComponent>("position_component",
        "x", &PositionComponent::x,
        "y", &PositionComponent::y,
        "z", &PositionComponent::z,
        "width", &PositionComponent::width,
        "height", &PositionComponent::height
    );
    state.declareComponentGetter<PositionComponent>("position");
}

std::ostream& operator<<(std::ostream& stream, const PositionComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    stream << "x=" << component.x << ", y=" << component.y << ", width=" <<
        component.width << ", height=" << component.height << std::endl;
    return stream;
}

}
