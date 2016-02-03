#ifndef YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H
#define YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H

#include "entityx/entityx.h"


namespace systems
{

class PlatformerSystem : public entityx::System<PlatformerSystem>
{
public:
    PlatformerSystem();

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
};

}

#endif
