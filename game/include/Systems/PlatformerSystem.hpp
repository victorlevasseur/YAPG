#ifndef YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H
#define YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H

#include <memory>

#include "entityx/entityx.h"

#include "Box2D/Dynamics/b2World.h"
#include "Box2D/Dynamics/b2WorldCallbacks.h"

#include "Collision/EntitySpatialGrid.hpp"
#include "Messaging/Messaging.hpp"

namespace systems
{

class PlatformerSystem : public entityx::System<PlatformerSystem>, public messaging::Emitter
{
public:
    PlatformerSystem(collision::EntitySpatialGrid& quadtreesGrid);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
    collision::EntitySpatialGrid& m_quadtreesGrid;

    std::unique_ptr<b2World> m_world;
};

}

#endif
