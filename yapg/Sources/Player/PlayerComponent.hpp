#ifndef YAPG_GAME_COMPONENTS_PLAYERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLAYERCOMPONENT_H

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Messaging/Messaging.hpp"
#include "Meta/Metadata.hpp"

namespace yapg
{

class PlayerComponent : public Component, public messaging::Emitter
{
public:
    PlayerComponent(entityx::Entity entity);
    virtual ~PlayerComponent();

    virtual std::string getName() const;

    static void registerComponent(LuaState& state);

    int playerNumber;

    bool hasFinishedLevel() const { return m_finishedLevel; }
    void setFinishedLevel();

private:
    bool m_finishedLevel;
};

std::ostream& operator<<(std::ostream& stream, const PlayerComponent& component);

}

#endif
