#ifndef YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H
#define YAPG_GAME_SYSTEMS_HITBOXUPDATERSYSTEM_H

#include "entityx/entityx.h"

#include <SFML/Graphics/Rect.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"

namespace systems
{

class HitboxUpdaterSystem : public entityx::System<HitboxUpdaterSystem>, public entityx::Receiver<HitboxUpdaterSystem>
{
public:
    HitboxUpdaterSystem();

    void configure(entityx::EventManager &event_manager);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:

};

}

#endif
