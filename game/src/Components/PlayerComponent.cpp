#include "Components/PlayerComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

PlayerComponent::PlayerComponent() :
    Component(),
    playerNumber(-1),
    finishedLevel(false)
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
    meta::MetadataStore::registerClass<PlayerComponent>("PlayerComponent")
        .declareAttribute("player_number", &PlayerComponent::playerNumber, false, true, false);

    lua::EntityHandle::declareComponent<PlayerComponent>("Player");

    state.getState().new_usertype<PlayerComponent>("player_component",
        "player_number", sol::readonly(&PlayerComponent::playerNumber),
        "finished_level", &PlayerComponent::finishedLevel
    );
}

std::ostream& operator<<(std::ostream& stream, const PlayerComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
