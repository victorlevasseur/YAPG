#ifndef YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H
#define YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H

#include "entityx/entityx.h"

#include <SFML/Graphics/Rect.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Tools/InfiniteQuadTreesGrid.hpp"

namespace systems
{

class HitboxUpdaterSystem : public entityx::System<HitboxUpdaterSystem>, public entityx::Receiver<HitboxUpdaterSystem>
{
public:
    using InfiniteQuadTreesGrid = tools::EntitySpatialGrid;

    HitboxUpdaterSystem();

    /**
     * Init the HitboxUpdaterSystem with the already existing entities
     */
    void initWithExistingEntities(entityx::EntityManager &es);

    void configure(entityx::EventManager &event_manager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    void receive(const entityx::ComponentRemovedEvent<components::PositionComponent>& event);
    void receive(const entityx::ComponentRemovedEvent<components::HitboxComponent>& event);
    void receive(const entityx::EntityDestroyedEvent& event);

    const tools::EntitySpatialGrid& getQuadTrees() const { return m_grid; }
    tools::EntitySpatialGrid& getQuadTrees() { return m_grid; }

private:
    void tryToAddEntityToQuadTree(entityx::Entity entity);
    void tryToRemoveEntityFromQuadTree(entityx::Entity entity);

    tools::EntitySpatialGrid m_grid;
};

}

#endif
