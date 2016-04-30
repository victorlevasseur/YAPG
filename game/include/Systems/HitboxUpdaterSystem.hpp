#ifndef YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H
#define YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H

#include "entityx/entityx.h"

#include <SFML/Graphics/Rect.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Tools/InfiniteQuadTreesGrid.hpp"

namespace systems
{

struct EntityAABBGetter
{
    static sf::FloatRect getAABB(const entityx::Entity& entity)
    {
        return sf::FloatRect(
            entity.component<const components::PositionComponent>()->x,
            entity.component<const components::PositionComponent>()->y,
            entity.component<const components::PositionComponent>()->width,
            entity.component<const components::PositionComponent>()->height
        );
    }
};

class HitboxUpdaterSystem : public entityx::System<HitboxUpdaterSystem>, public entityx::Receiver<HitboxUpdaterSystem>
{
public:
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

    const tools::InfiniteQuadTreesGrid<entityx::Entity, EntityAABBGetter>& getQuadTrees() const { return m_quadtreesGrid; }
    tools::InfiniteQuadTreesGrid<entityx::Entity, EntityAABBGetter>& getQuadTrees() { return m_quadtreesGrid; }

private:
    void tryToAddEntityToQuadTree(entityx::Entity entity);
    void tryToRemoveEntityFromQuadTree(entityx::Entity entity);

    tools::InfiniteQuadTreesGrid<entityx::Entity, EntityAABBGetter> m_quadtreesGrid;
};

}

#endif
