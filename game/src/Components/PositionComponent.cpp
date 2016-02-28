#include "Components/PositionComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

PositionComponent::PositionComponent() :
    Component(),
    x(0.f),
    y(0.f),
    z(0.f),
    width(0.f),
    height(0.f)
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

    lua::EntityHandle::declareComponent<PositionComponent>("Position");

    //Register to lua
    state.getState().new_usertype<PositionComponent>("position_component");
}

std::ostream& operator<<(std::ostream& stream, const PositionComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    stream << "x=" << component.x << ", y=" << component.y << ", width=" <<
        component.width << ", height=" << component.height << std::endl;
    return stream;
}

}
