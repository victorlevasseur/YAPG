#ifndef YAPG_GAME_SYSTEMS_ENTITYGRIDSYSTEM_H
#define YAPG_GAME_SYSTEMS_ENTITYGRIDSYSTEM_H

#include "entityx/entityx.h"

#include <SFML/Graphics/Rect.hpp>

#include "Collision/EntitySpatialGrid.hpp"
#include "Components/PositionComponent.hpp"

namespace systems
{

class EntityGridSystem : public entityx::System<EntityGridSystem>, public entityx::Receiver<EntityGridSystem>
{
public:
    EntityGridSystem();

    /**
     * Init the EntityGridSystem with the already existing entities
     */
    void initWithExistingEntities(entityx::EntityManager &es);

    void configure(entityx::EventManager &event_manager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    void receive(const entityx::ComponentRemovedEvent<components::PositionComponent>& event);
    void receive(const entityx::EntityDestroyedEvent& event);

    const collision::EntitySpatialGrid& getGrid() const { return m_grid; }
    collision::EntitySpatialGrid& getGrid() { return m_grid; }

private:
    void tryToAddEntityToGrid(entityx::Entity entity);
    void tryToRemoveEntityFromGrid(entityx::Entity entity);

    collision::EntitySpatialGrid m_grid;
};

}

#endif
