#ifndef YAPG_GAME_SYSTEMS_PLAYERSYSTEM_H
#define YAPG_GAME_SYSTEMS_PLAYERSYSTEM_H

#include "entityx/entityx.h"

#include "Health/HealthSystem.hpp"
#include "Messaging/LevelMessages.hpp"
#include "Messaging/Messaging.hpp"

namespace settings{ class SettingsManager; }

namespace systems
{

struct PlayerFinishedMessage
{
    entityx::Entity player;
};

class PlayerSystem : public entityx::System<PlayerSystem>, public messaging::Receiver<PlayerFinishedMessage, HealthKilledMessage>, public messaging::Emitter
{
public:
    PlayerSystem(std::size_t playersCount, const settings::SettingsManager& settingsManager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    void receive(const PlayerFinishedMessage& msg);
    void receive(const HealthKilledMessage& msg);

private:
    std::size_t m_playersCount;
    std::size_t m_stillAliveCount;
    std::size_t m_finishedCount;

    void sendLevelMessages();

    const settings::SettingsManager& m_settingsManager;
};

}

#endif
