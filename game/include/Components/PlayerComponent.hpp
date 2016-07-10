#ifndef YAPG_GAME_COMPONENTS_PLAYERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLAYERCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Messaging/Messaging.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class PlayerComponent : public Component, public messaging::Emitter
{
public:
    PlayerComponent(entityx::Entity entity);
    virtual ~PlayerComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    int playerNumber;

    bool hasFinishedLevel() const { return m_finishedLevel; }
    void setFinishedLevel();

private:
    bool m_finishedLevel;
};

std::ostream& operator<<(std::ostream& stream, const PlayerComponent& component);

}

#endif
