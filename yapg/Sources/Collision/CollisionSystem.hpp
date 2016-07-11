#ifndef YAPG_GAME_SYSTEMS_COLLISIONSYSTEM_H
#define YAPG_GAME_SYSTEMS_COLLISIONSYSTEM_H

#include <map>
#include <unordered_set>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "entityx/entityx.h"

#include "Common/EntitySpatialGrid.hpp"
#include "Messaging/Messaging.hpp"

namespace yapg
{

class CollisionSystem : public entityx::System<CollisionSystem>
{
    struct Collision
    {
        struct Hash
        {
            std::size_t operator()(Collision const& c) const
            {
                std::size_t h1 = std::hash<entityx::Entity>()(c.entity1);
                std::size_t h2 = std::hash<std::string>()(c.polygonName1);
                std::size_t h3 = std::hash<entityx::Entity>()(c.entity2);
                std::size_t h4 = std::hash<std::string>()(c.polygonName2);
                return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
            }
        };

        entityx::Entity entity1;
        std::string polygonName1;

        entityx::Entity entity2;
        std::string polygonName2;

        bool operator==(const Collision& other) const
        {
            return (entity1 == other.entity1 && polygonName1 == other.polygonName1 && entity2 == other.entity2 && polygonName2 == other.polygonName2)
                || (entity1 == other.entity2 && polygonName1 == other.polygonName2 && entity1 == other.entity2 && polygonName1 == other.polygonName2);
        }
    };

    using CollisionUnorderedSet = std::unordered_set<Collision, Collision::Hash>;

public:
    CollisionSystem(EntitySpatialGrid& quadtreesGrid);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    static sf::FloatRect comparePoints(const sf::Vector2f& point) {return sf::FloatRect(point.x, point.y, 1.f, 1.f);}

private:
    CollisionUnorderedSet m_collisions; ///< The collisions that are currently happenning.

    EntitySpatialGrid& m_quadtreesGrid;
};

}

#endif
