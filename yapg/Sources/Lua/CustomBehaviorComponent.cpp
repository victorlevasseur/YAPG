#include "Lua/CustomBehaviorComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace yapg
{

CustomBehaviorComponent::CustomBehaviorComponent(entityx::Entity entity) :
    Component(entity),
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

void CustomBehaviorComponent::registerComponent(LuaState& state)
{
    meta::MetadataStore::registerClass<CustomBehaviorComponent>("CustomBehaviorComponent")
        .declareAttribute<sol::function>("on_update", &CustomBehaviorComponent::updateFunction);

    EntityHandle::declareComponent<CustomBehaviorComponent>("custom_behavior");

    state.getState().new_usertype<CustomBehaviorComponent>("custom_behavior_component",
        "on_update", &CustomBehaviorComponent::updateFunction
    );
    state.declareComponentGetter<CustomBehaviorComponent>("custom_behavior");
}

std::ostream& operator<<(std::ostream& stream, const CustomBehaviorComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    return stream;
}

}
