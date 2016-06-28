#include "Systems/CollisionSystem.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Collision/Polygon.hpp"
#include "Components/CollidableComponent.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/PlatformerHitboxComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Lua/EntityHandle.hpp"

namespace c = components;

namespace systems
{

CollisionSystem::CollisionSystem(collision::EntitySpatialGrid& quadtreesGrid) :
    m_collisions(),
    m_quadtreesGrid(quadtreesGrid)
{

}

void CollisionSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    CollisionUnorderedSet nextFrameCollisions(m_collisions);

    es.each<c::PositionComponent, c::CollidableComponent>([&](
        entityx::Entity entity1,
        c::PositionComponent& position1,
        c::CollidableComponent& collidable1)
    {
        sf::FloatRect boundingBox(position1.x, position1.y, position1.width, position1.height);

        auto collisionCandidates = m_quadtreesGrid.getEntitiesIntersectingAABB(boundingBox);
        for(entityx::Entity entity2 : collisionCandidates)
        {
            if(!entity1 || !entity2) //Prevents crashes when an entity is deleted as a result of a callback call.
                break;

            if(!entity2.has_component<c::CollidableComponent>() || entity1 == entity2)
                continue;

            auto position2 = entity2.component<c::PositionComponent>();
            auto collidable2 = entity2.component<c::CollidableComponent>();

            for(auto& polygon1 : collidable1.polygons)
            {
                if(!entity1 || !entity2) //Prevents crashes when an entity is deleted as a result of a callback call.
                    break;
                //TODO: Find a way to defer the callback calls to avoid crashes when iterating through entities when they are deleted in callbacks

                bool stopIterating = false;

                for(auto& polygon2 : collidable2->polygons)
                {
                    if(!entity1 || !entity2) //Prevents crashes when an entity is deleted as a result of a callback call.
                        break;

                    if(collision::Polygon::collides(polygon1.second.getPolygon(), polygon2.second.getPolygon(), position1.getPositionTransform(), position2->getPositionTransform())) //Collision between the two polygons
                    {
                        //If it's an polygon1 is exclusive, do not iterate the others
                        if(polygon1.second.isExclusive())
                            stopIterating = true;

                        if(nextFrameCollisions.insert(Collision{entity1, polygon1.first, entity2, polygon2.first}).second) //They were not colliding before !
                        {
                            std::cout << "Collision begin: " << entity1 << " -> " << polygon1.first << " <=> " << entity2 << " -> " << polygon2.first << std::endl;
                            polygon1.second.callOnCollisionBegin(entity1, entity2);
                            std::cout << "OK !" << std::endl;
                        }
                        else //They were already colliding before
                        {
                            std::cout << "Collision in progress: " << entity1 << " -> " << polygon1.first << " <=> " << entity2 << " -> " << polygon2.first << std::endl;
                            polygon1.second.callCollides(entity1, entity2);
                        }
                    }
                    else
                    {
                        if(nextFrameCollisions.erase(Collision{entity1, polygon1.first, entity2, polygon2.first}) == 1) //The collision has just ended
                        {
                            std::cout << "Collision ended: " << entity1 << " -> " << polygon1.first << " <=> " << entity2 << " -> " << polygon2.first << std::endl;
                            polygon1.second.callOnCollisionEnd(entity1, entity2);
                        }
                    }

                    if(stopIterating)
                        break;
                }
            }
        }
    });

    m_collisions = nextFrameCollisions;
}

}
