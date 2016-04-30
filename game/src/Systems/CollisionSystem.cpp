#include "Systems/CollisionSystem.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Components/CollidableComponent.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Lua/EntityHandle.hpp"
#include "Tools/Polygon.hpp"

namespace c = components;

namespace systems
{

CollisionSystem::CollisionSystem(HitboxUpdaterSystem::InfiniteQuadTreesGrid& quadtreesGrid) :
    m_entitiesInCollision(),
    m_declaredCollisions(),
    m_quadtreesGrid(quadtreesGrid)
{

}

void CollisionSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<c::PositionComponent, c::HitboxComponent, c::ColliderComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position,
        c::HitboxComponent& hitbox,
        c::ColliderComponent& collider)
    {
        sf::FloatRect boundingBox(position.x, position.y, position.width, position.height);

        es.each<c::PositionComponent, c::HitboxComponent, c::CollidableComponent>([&](
            entityx::Entity entity2,
            c::PositionComponent& position2,
            c::HitboxComponent& hitbox2,
            c::CollidableComponent& collidable2)
        {
            if(entity == entity2)
                return;

            auto collisionPairIt = std::find(m_entitiesInCollision.begin(), m_entitiesInCollision.end(), std::make_pair(entity, entity2));
            sf::FloatRect boundingBox2(position2.x, position2.y, position2.width, position2.height);

            if((!boundingBox.intersects(boundingBox2) ||
                !PolygonCollision(hitbox.getHitbox(), hitbox2.getHitbox())) &&
                std::find(m_declaredCollisions.begin(), m_declaredCollisions.end(), std::make_pair(entity, entity2)) == m_declaredCollisions.end())
            {
                //No collision (or no more collision)

                //If the pair (collider, collidable) was colliding before, remove it from the entities colliding and call on_end_collision
                if(collisionPairIt != m_entitiesInCollision.end())
                {
                    if(collidable2.onCollisionEnd.valid())
                        collidable2.onCollisionEnd.call(lua::EntityHandle(entity2), lua::EntityHandle(entity));

                    //If the collider also has a CollidableComponent, call the callback on it too
                    if(entity.has_component<c::CollidableComponent>())
                    {
                        if(entity.component<c::CollidableComponent>()->onCollisionEnd.valid())
                            entity.component<c::CollidableComponent>()->onCollisionEnd.call(lua::EntityHandle(entity), lua::EntityHandle(entity2));
                    }

                    m_entitiesInCollision.erase(collisionPairIt);
                }
            }
            else
            {
                //If the pair (collider, collidable) was not colliding before, add it to the entities colliding and call on_begin_collision
                if(collisionPairIt == m_entitiesInCollision.end())
                {
                    m_entitiesInCollision.push_back(std::make_pair(entity, entity2));
                    if(collidable2.onCollisionBegin.valid())
                        collidable2.onCollisionBegin.call(lua::EntityHandle(entity2), lua::EntityHandle(entity));

                    //If the collider also has a CollidableComponent, call the callback on it too
                    if(entity.has_component<c::CollidableComponent>())
                    {
                        if(entity.component<c::CollidableComponent>()->onCollisionBegin.valid())
                            entity.component<c::CollidableComponent>()->onCollisionBegin.call(lua::EntityHandle(entity), lua::EntityHandle(entity2));
                    }
                }
                else
                {
                    //Collision still happening, call collides callback
                    if(collidable2.collides.valid())
                        collidable2.collides.call(lua::EntityHandle(entity2), lua::EntityHandle(entity));

                    //If the collider also has a CollidableComponent, call the callback on it too
                    if(entity.has_component<c::CollidableComponent>())
                    {
                        if(entity.component<c::CollidableComponent>()->collides.valid())
                            entity.component<c::CollidableComponent>()->collides.call(lua::EntityHandle(entity), lua::EntityHandle(entity2));
                    }
                }
            }
        });
    });

    m_declaredCollisions.clear();
}

void CollisionSystem::receive(const ExtraSystemCollisionMessage& collisionMessage)
{
    //Check if they are respectively the ColliderComponent and the CollidableComponent
    if(collisionMessage.collider.has_component<c::ColliderComponent>() && collisionMessage.collidable.has_component<c::CollidableComponent>())
    {
        if(std::find(m_declaredCollisions.begin(), m_declaredCollisions.end(), std::make_pair(collisionMessage.collider, collisionMessage.collidable)) == m_declaredCollisions.end())
        {
            m_declaredCollisions.push_back(std::make_pair(collisionMessage.collider, collisionMessage.collidable));
        }
    }
}

}
