#include "Components/PositionComponent.hpp"

namespace game
{
namespace components
{

PositionComponent::PositionComponent() :
    Component(),
    x(0.f),
    y(0.f),
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
    meta::MetadataStore::registerType<PositionComponent>()
        .declareAttribute<float>("x", &PositionComponent::x)
        .declareAttribute<float>("y", &PositionComponent::y)
        .declareAttribute<float>("width", &PositionComponent::width)
        .declareAttribute<float>("height", &PositionComponent::height);

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
