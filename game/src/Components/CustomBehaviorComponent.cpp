#include "Components/CustomBehaviorComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

CustomBehaviorComponent::CustomBehaviorComponent() :
    Component(),
    updateFunction()
{

}

CustomBehaviorComponent::~CustomBehaviorComponent()
{

}

std::string CustomBehaviorComponent::getName() const
{
    return "CustomBehavior";
}

void CustomBehaviorComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<CustomBehaviorComponent>("CustomBehaviorComponent")
        .declareAttribute<sol::function>("on_update", &CustomBehaviorComponent::updateFunction);

    lua::EntityHandle::declareComponent<CustomBehaviorComponent>("CustomBehavior");

    state.getState().new_usertype<CustomBehaviorComponent>("custom_behavior_component",
        "on_update", &CustomBehaviorComponent::updateFunction
    );
}

std::ostream& operator<<(std::ostream& stream, const CustomBehaviorComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    return stream;
}

}
