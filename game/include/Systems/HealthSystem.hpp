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

class HealthSystem : public entityx::System<HealthSystem>, public messaging::Receiver<HealthKillMessage>
{
public:

    HealthSystem(const settings::SettingsManager& settingsManager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    virtual void receive(const HealthKillMessage& msg);

private:
    const settings::SettingsManager& m_settingsManager;

    std::vector<entityx::Entity> m_alreadyDeadEntities;
};

}

#endif
