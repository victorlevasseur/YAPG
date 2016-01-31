#include "Components/CustomBehaviorComponent.hpp"

namespace game
{
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

void CustomBehaviorComponent::loadFromLua(const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    updateFunction = luaTable.as<sol::table>().get<sol::function>("on_update");
}

void CustomBehaviorComponent::registerComponent(lua::LuaState& state)
{
    state.getState().new_usertype<CustomBehaviorComponent>("custom_behavior_component");
}

std::ostream& operator<<(std::ostream& stream, const CustomBehaviorComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    return stream;
}

}
}
