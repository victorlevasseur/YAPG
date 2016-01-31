#include "Components/PlatformComponent.hpp"

namespace game
{
namespace components
{

PlatformComponent::PlatformComponent() :
    Component()
{

}

PlatformComponent::~PlatformComponent()
{

}

std::string PlatformComponent::getName() const
{
    return "Platform";
}

void PlatformComponent::loadFromLua(const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    //TODO: Load from lua table
}

void PlatformComponent::registerComponent(lua::LuaState& state)
{
    state.getState().new_usertype<PlatformComponent>("platform_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const PlatformComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
}