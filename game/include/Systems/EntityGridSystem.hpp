#ifndef YAPG_GAME_SYSTEMS_ENTITYGRIDSYSTEM_H
#define YAPG_GAME_SYSTEMS_ENTITYGRIDSYSTEM_H

#include "entityx/entityx.h"

#include <SFML/Graphics/Rect.hpp>

#include "Components/PositionComponent.hpp"
#include "Tools/EntitySpatialGrid.hpp"

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

    const tools::EntitySpatialGrid& getGrid() const { return m_grid; }
    tools::EntitySpatialGrid& getGrid() { return m_grid; }

private:
    void tryToAddEntityToGrid(entityx::Entity entity);
    void tryToRemoveEntityFromGrid(entityx::Entity entity);

    tools::EntitySpatialGrid m_grid;
};

}

#endif
