#ifndef YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H
#define YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H

#include "entityx/entityx.h"

namespace systems{ class CollisionSystem; }

namespace systems
{

class PlatformerSystem : public entityx::System<PlatformerSystem>
{
public:
    PlatformerSystem(CollisionSystem& collisionSystem);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
    CollisionSystem& m_collisionSystem;
};

}

#endif
