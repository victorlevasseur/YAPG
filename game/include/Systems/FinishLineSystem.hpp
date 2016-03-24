#ifndef YAPG_GAME_SYSTEMS_FINISHLINESYSTEM_H
#define YAPG_GAME_SYSTEMS_FINISHLINESYSTEM_H

#include "entityx/entityx.h"

#include "Tools/Observable.hpp"

namespace systems
{

class FinishLineSystem : public entityx::System<FinishLineSystem>
{
public:
    FinishLineSystem();

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    bool haveAllPlayersFinished() const;

private:
    bool m_allPlayersFinishedLevel;
};

}

#endif
