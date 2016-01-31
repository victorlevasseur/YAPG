#include "Components/CustomBehaviorComponent.hpp"

#include "Lua/Loader.hpp"

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
    lua::MetadataStore::getMetadata<CustomBehaviorComponent>().load(this, luaTable);
}

void CustomBehaviorComponent::registerComponent(lua::LuaState& state)
{
    lua::MetadataStore::registerClass<CustomBehaviorComponent>()
        .declareLoadableAttribute<sol::function>("on_update", &CustomBehaviorComponent::updateFunction);

    state.getState().new_usertype<CustomBehaviorComponent>("custom_behavior_component");
}

std::ostream& operator<<(std::ostream& stream, const CustomBehaviorComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    return stream;
}

}
}
