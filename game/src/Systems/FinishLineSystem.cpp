#include "Systems/FinishLineSystem.hpp"

#include "Components/FinishLineComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Tools/Polygon.hpp"

namespace c = components;

namespace systems
{

FinishLineSystem::FinishLineSystem() :
    m_allPlayersFinishedLevel(false)
{

}

void FinishLineSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    m_allPlayersFinishedLevel = false;
    es.each<c::HitboxComponent, c::FinishLineComponent>([&](entityx::Entity, c::HitboxComponent&, c::FinishLineComponent&)
    {
        m_allPlayersFinishedLevel = true; //Be sure to have at least one finish line entity !
    });

    es.each<c::PositionComponent, c::HitboxComponent, c::PlayerComponent>([&](entityx::Entity entity, c::PositionComponent& position, c::HitboxComponent& hitbox, c::PlayerComponent& player)
    {
        es.each<c::HitboxComponent, c::FinishLineComponent>([&](entityx::Entity finishLineEntity, c::HitboxComponent& finishLineHitbox, c::FinishLineComponent&)
        {
            if(!player.finishedLevel)
            {
                if(tools::PolygonCollision(hitbox.getHitbox(), finishLineHitbox.getHitbox()))
                {
                    //The player has collided the finish line.
                    player.finishedLevel = true;
                }
                else
                {
                    m_allPlayersFinishedLevel = false;
                }
            }
        });
    });
}

bool FinishLineSystem::haveAllPlayersFinished() const
{
    return m_allPlayersFinishedLevel;
}

}