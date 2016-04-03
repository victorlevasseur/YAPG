#ifndef YAPG_GAME_SYSTEMS_PLAYERSYSTEM_H
#define YAPG_GAME_SYSTEMS_PLAYERSYSTEM_H

#include "entityx/entityx.h"

#include "Messaging/Messaging.hpp"

namespace settings{ class SettingsManager; }

namespace systems
{

struct PlayerFallingDeathMessage
{
    entityx::Entity player;
};

class PlayerSystem : public entityx::System<PlayerSystem>, public messaging::Emitter, public messaging::Receiver<PlayerFallingDeathMessage>
{
public:
    PlayerSystem(const settings::SettingsManager& settingsManager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    virtual void receive(const PlayerFallingDeathMessage& msg);

private:
    const settings::SettingsManager& m_settingsManager;
};

}

#endif
