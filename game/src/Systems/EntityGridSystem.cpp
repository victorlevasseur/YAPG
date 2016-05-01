#include "Systems/EntityGridSystem.hpp"

#include <SFML/System/Vector2.hpp>

namespace c = components;

namespace systems
{

EntityGridSystem::EntityGridSystem() :
    m_grid(250.f, 250.f)
{

}

void EntityGridSystem::initWithExistingEntities(entityx::EntityManager &es)
{
    es.each<c::PositionComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position)
    {
        tryToAddEntityToGrid(entity);
    });
}

void EntityGridSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<entityx::ComponentRemovedEvent<c::PositionComponent>>(*this);
    event_manager.subscribe<entityx::EntityDestroyedEvent>(*this);
}

void EntityGridSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
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
    });
}

void EntityGridSystem::receive(const entityx::ComponentRemovedEvent<components::PositionComponent>& event)
{
    tryToRemoveEntityFromGrid(event.entity);
}

void EntityGridSystem::receive(const entityx::EntityDestroyedEvent& event)
{
    tryToRemoveEntityFromGrid(event.entity);
}

void EntityGridSystem::tryToAddEntityToGrid(entityx::Entity entity)
{
    if(entity.has_component<c::PositionComponent>())
    {
        m_grid.insert(entity);
    }
}

void EntityGridSystem::tryToRemoveEntityFromGrid(entityx::Entity entity)
{
    m_grid.erase(entity);
}

}
