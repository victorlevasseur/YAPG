#include "Components/PlatformerComponent.hpp"

namespace game
{
namespace components
{

PlatformerComponent::PlatformerComponent() :
    Component()
{

}

PlatformerComponent::~PlatformerComponent()
{

}

std::string PlatformerComponent::getName() const
{
    return "Platformer";
}

void PlatformerComponent::loadFromLua(const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    //TODO: Load from lua table
}

void PlatformerComponent::registerComponent(lua::LuaState& state)
{
    state.getState().new_usertype<PlatformerComponent>("platformer_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const PlatformerComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
}