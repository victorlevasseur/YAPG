#ifndef YAPG_GAME_SYSTEMS_CUSTOMBEHAVIORSYSTEM_H
#define YAPG_GAME_SYSTEMS_CUSTOMBEHAVIORSYSTEM_H

#include "entityx/entityx.h"

namespace systems
{

class CustomBehaviorSystem : public entityx::System<CustomBehaviorSystem>
{
public:
    CustomBehaviorSystem();

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
};

}

#endif
