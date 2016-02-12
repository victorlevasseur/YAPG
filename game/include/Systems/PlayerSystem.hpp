#ifndef YAPG_GAME_SYSTEMS_PLAYERSYSTEM_H
#define YAPG_GAME_SYSTEMS_PLAYERSYSTEM_H

#include "entityx/entityx.h"

namespace settings{ class SettingsManager; }

namespace systems
{

class PlayerSystem : public entityx::System<PlayerSystem>
{
public:
    PlayerSystem(const settings::SettingsManager& settingsManager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
    const settings::SettingsManager& m_settingsManager;
};

}

#endif
