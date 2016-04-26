#ifndef YAPG_GAME_SYSTEMS_HEALTHSYSTEM_H
#define YAPG_GAME_SYSTEMS_HEALTHSYSTEM_H

#include "entityx/entityx.h"

#include "Messaging/Messaging.hpp"

namespace settings{ class SettingsManager; }

namespace systems
{

struct HealthKillMessage
{
    entityx::Entity entityToKill;
};

struct HealthLoosePVMessage
{
    entityx::Entity entity;
    float pv;
};

struct HealthGainPVMessage
{
    entityx::Entity entity;
    float pv;
};

class HealthSystem : public entityx::System<HealthSystem>, public messaging::Receiver<HealthKillMessage, HealthLoosePVMessage, HealthGainPVMessage>
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
