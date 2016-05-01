#ifndef YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H
#define YAPG_GAME_SYSTEMS_PLATFORMERSYSTEM_H

#include "entityx/entityx.h"

#include "Messaging/Messaging.hpp"
#include "Tools/EntitySpatialGrid.hpp"

namespace systems
{

class PlatformerSystem : public entityx::System<PlatformerSystem>, public messaging::Emitter
{
public:
    PlatformerSystem(tools::EntitySpatialGrid& quadtreesGrid);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
    tools::EntitySpatialGrid& m_quadtreesGrid;
};

}

#endif
