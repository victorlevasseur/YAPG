#ifndef YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H
#define YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H

#include "entityx/entityx.h"

namespace systems
{

class HitboxUpdaterSystem : public entityx::System<HitboxUpdaterSystem>
{
public:
    HitboxUpdaterSystem();

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:

};

}

#endif
