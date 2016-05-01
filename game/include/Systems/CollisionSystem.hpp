#ifndef YAPG_GAME_SYSTEMS_COLLISIONSYSTEM_H
#define YAPG_GAME_SYSTEMS_COLLISIONSYSTEM_H

#include <map>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "entityx/entityx.h"

#include "Messaging/Messaging.hpp"
#include "Systems/HitboxUpdaterSystem.hpp"
#include "Tools/EntitySpatialGrid.hpp"

namespace systems
{

/**
 * Struct used by other systems and classes to send col²lisions messages to the
 * CollisionSystem.
 */
struct ExtraSystemCollisionMessage
{
    entityx::Entity collider;
    entityx::Entity collidable;
};



class CollisionSystem : public entityx::System<CollisionSystem>, public messaging::Receiver<ExtraSystemCollisionMessage>
{
public:
    CollisionSystem(HitboxUpdaterSystem::InfiniteQuadTreesGrid& quadtreesGrid);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    virtual void receive(const ExtraSystemCollisionMessage& collisionMessage) override;

    static sf::FloatRect comparePoints(const sf::Vector2f& point) {return sf::FloatRect(point.x, point.y, 1.f, 1.f);}

private:
    std::vector<std::pair<entityx::Entity, entityx::Entity>> m_entitiesInCollision; ///< The first of the pair is the Collider and the second is the Collidable.

    std::vector<std::pair<entityx::Entity, entityx::Entity>> m_declaredCollisions; ///< Stores collision declared using the declareCollision(...) method (the PlatformerSystem use it)

    HitboxUpdaterSystem::InfiniteQuadTreesGrid& m_quadtreesGrid;
};

}

#endif
