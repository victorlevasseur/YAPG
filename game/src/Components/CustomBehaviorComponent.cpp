#include "Components/CustomBehaviorComponent.hpp"


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
    meta::MetadataStore::registerClass<CustomBehaviorComponent>()
        .declareAttribute<sol::function>("on_update", &CustomBehaviorComponent::updateFunction);

    state.getState().new_usertype<CustomBehaviorComponent>("custom_behavior_component");
}

std::ostream& operator<<(std::ostream& stream, const CustomBehaviorComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    return stream;
}

}
