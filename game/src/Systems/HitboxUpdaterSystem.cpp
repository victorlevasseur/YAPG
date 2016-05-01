#include "Systems/HitboxUpdaterSystem.hpp"

#include <SFML/System/Vector2.hpp>

namespace c = components;

namespace systems
{

HitboxUpdaterSystem::HitboxUpdaterSystem() :
    m_grid(250.f, 250.f)
{

}

void HitboxUpdaterSystem::initWithExistingEntities(entityx::EntityManager &es)
{
    es.each<c::PositionComponent, c::HitboxComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position,
        c::HitboxComponent& hitbox)
    {
        tryToAddEntityToQuadTree(entity);
    });
}

void HitboxUpdaterSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<entityx::ComponentRemovedEvent<c::PositionComponent>>(*this);
    event_manager.subscribe<entityx::EntityDestroyedEvent>(*this);
}

void HitboxUpdaterSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<c::PositionComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position)
    {
        if(!m_grid.contains(entity))
        {
            m_grid.insert(entity);
        }

        if(position.x != position.oldX
            || position.y != position.oldY
            || position.width != position.oldWidth
            || position.height != position.oldHeight)
        {
            m_grid.update(entity);

            position.oldX = position.x;
            position.oldY = position.y;
            position.oldWidth = position.width;
            position.oldHeight = position.height;
        }

        if(entity.has_component<c::HitboxComponent>())
        {
            auto hitbox = entity.component<c::HitboxComponent>();
            if(hitbox->getHitbox().GetOrigin() != sf::Vector2f(position.x, position.y))
            {
                hitbox->getHitbox().SetOrigin(sf::Vector2f(position.x, position.y));
                hitbox->getHitbox().ComputeGlobalVertices();
                hitbox->getHitbox().ComputeGlobalEdges();
                hitbox->getHitbox().ComputeGlobalCenter();
            }
        }
    });
}

void HitboxUpdaterSystem::receive(const entityx::ComponentRemovedEvent<components::PositionComponent>& event)
{
    tryToRemoveEntityFromQuadTree(event.entity);
}

void HitboxUpdaterSystem::receive(const entityx::EntityDestroyedEvent& event)
{
    tryToRemoveEntityFromQuadTree(event.entity);
}

void HitboxUpdaterSystem::tryToAddEntityToQuadTree(entityx::Entity entity)
{
    if(entity.has_component<c::PositionComponent>())
    {
        m_grid.insert(entity);
    }
}

void HitboxUpdaterSystem::tryToRemoveEntityFromQuadTree(entityx::Entity entity)
{
    m_grid.erase(entity);
}

}
