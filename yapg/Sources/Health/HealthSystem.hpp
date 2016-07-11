#ifndef YAPG_GAME_SYSTEMS_HEALTHSYSTEM_H
#define YAPG_GAME_SYSTEMS_HEALTHSYSTEM_H

#include "entityx/entityx.h"

#include "Messaging/Messaging.hpp"

namespace settings{ class SettingsManager; }

namespace systems
{

/**
 * Message to send to ask the HealthSystem to try to kill an entity.
 */
struct HealthKillMessage
{
    entityx::Entity entityToKill;
};

/**
 * Message to send to ask the HealthSystem to try to remove PV of an entity.
 */
struct HealthLoosePVMessage
{
    entityx::Entity entity;
    float pv;
};

/**
 * Message to send to ask the HealthSystem to try to add PV to an entity.
 */
struct HealthGainPVMessage
{
    entityx::Entity entity;
    float pv;
};

/**
 * Message sent by the HealthSystem to inform that an entity is about to be killed (and will be for sure).
 */
struct HealthKilledMessage
{
    entityx::Entity entity;
};

class HealthSystem : public entityx::System<HealthSystem>, public messaging::Emitter, public messaging::Receiver<HealthKillMessage, HealthLoosePVMessage, HealthGainPVMessage>
{
public:

    HealthSystem(const settings::SettingsManager& settingsManager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    virtual void receive(const HealthKillMessage& msg);
    virtual void receive(const HealthLoosePVMessage& msg);
    virtual void receive(const HealthGainPVMessage& msg);

private:
    const settings::SettingsManager& m_settingsManager;

    std::vector<entityx::Entity> m_alreadyDeadEntities;
};

}

#endif
