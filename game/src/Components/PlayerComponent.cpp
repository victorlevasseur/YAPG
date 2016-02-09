#include "Components/PlayerComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

PlayerComponent::PlayerComponent() :
    Component()
{

}

PlayerComponent::~PlayerComponent()
{

}

std::string PlayerComponent::getName() const
{
    return "Player";
}

void PlayerComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<PlayerComponent>()
        .declareAttribute("player_number", &PlayerComponent::playerNumber, false, true, false);

    lua::EntityHandle::declareComponent<PlayerComponent>("Player");

    state.getState().new_usertype<PlayerComponent>("player_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const PlayerComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
