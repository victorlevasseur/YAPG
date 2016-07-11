#include "Player/PlayerComponent.hpp"

#include "Lua/EntityHandle.hpp"
#include "Player/PlayerSystem.hpp"

namespace components
{

PlayerComponent::PlayerComponent(entityx::Entity entity) :
    Component(entity),
    playerNumber(-1),
    m_finishedLevel(false)
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

    lua::EntityHandle::declareComponent<PlayerComponent>("player");

    state.getState().new_usertype<PlayerComponent>("player_component",
        "player_number", sol::readonly(&PlayerComponent::playerNumber),
        "has_finished_level", &PlayerComponent::hasFinishedLevel,
        "set_finished_level", &PlayerComponent::setFinishedLevel
    );
    state.declareComponentGetter<PlayerComponent>("player");
}

void PlayerComponent::setFinishedLevel()
{
    m_finishedLevel = true;
    emit<systems::PlayerFinishedMessage>(getEntity());
}

std::ostream& operator<<(std::ostream& stream, const PlayerComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
