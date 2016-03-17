#ifndef YAPG_GAME_SYSTEMS_COLLISIONSYSTEM_H
#define YAPG_GAME_SYSTEMS_COLLISIONSYSTEM_H

#include <map>
#include <vector>

#include "entityx/entityx.h"

namespace systems
{

class CollisionSystem : public entityx::System<CollisionSystem>
{
public:
    CollisionSystem();

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    void declareCollision(entityx::Entity collider, entityx::Entity collidable);

private:
    std::vector<std::pair<entityx::Entity, entityx::Entity>> m_entitiesInCollision; ///< The first of the pair is the Collider and the second is the Collidable.

    std::vector<std::pair<entityx::Entity, entityx::Entity>> m_declaredCollisions; ///< Stores collision declared using the declareCollision(...) method (the PlatformerSystem use it)
};

}

#endif
