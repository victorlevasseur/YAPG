#include "Systems/FinishLineSystem.hpp"

#include "Collision/Polygon.hpp"
#include "Components/FinishLineComponent.hpp"
#include "Components/PlatformerHitboxComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"

namespace c = components;

namespace systems
{

FinishLineSystem::FinishLineSystem() :
    m_allPlayersFinishedLevel(false)
{

}

void FinishLineSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    bool wereAllPlayersFinished = m_allPlayersFinishedLevel;

    m_allPlayersFinishedLevel = false;
    es.each<c::PlatformerHitboxComponent, c::FinishLineComponent>([&](entityx::Entity, c::PlatformerHitboxComponent&, c::FinishLineComponent&)
    {
        m_allPlayersFinishedLevel = true; //Be sure to have at least one finish line entity !
    });

    es.each<c::PositionComponent, c::PlatformerHitboxComponent, c::PlayerComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position,
        c::PlatformerHitboxComponent& hitbox,
        c::PlayerComponent& player)
    {
        es.each<c::PositionComponent, c::PlatformerHitboxComponent, c::FinishLineComponent>([&](
            entityx::Entity finishLineEntity,
            c::PositionComponent& finishLinePosition,
            c::PlatformerHitboxComponent& finishLineHitbox,
            c::FinishLineComponent&)
        {
            if(!player.finishedLevel)
            {
                if(collision::Polygon::collides(hitbox.getHitbox(), finishLineHitbox.getHitbox(), position.getPositionTransform(), finishLinePosition.getPositionTransform()))
                {
                    //The player has collided the finish line.
                    player.finishedLevel = true;
                    emit<messaging::PlayerFinishedMessage>(player.playerNumber);
                }
                else
                {
                    m_allPlayersFinishedLevel = false;
                }
            }
        });
    });

    if(!wereAllPlayersFinished && m_allPlayersFinishedLevel)
        emit<messaging::AllPlayersFinishedMessage>();
}

bool FinishLineSystem::haveAllPlayersFinished() const
{
    return m_allPlayersFinishedLevel;
}

}
