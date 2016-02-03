#include "Systems/HitboxUpdaterSystem.hpp"

#include <SFML/System/Vector2.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"

namespace c = components;

namespace systems
{

HitboxUpdaterSystem::HitboxUpdaterSystem()
{

}

void HitboxUpdaterSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<c::PositionComponent, c::HitboxComponent>([&](
        entityx::Entity entity,
        c::PositionComponent& position,
        c::HitboxComponent& hitbox)
    {
        if(hitbox.getHitbox().GetOrigin() == sf::Vector2f(position.x, position.y))
        {
            return;
        }

        hitbox.getHitbox().SetOrigin(sf::Vector2f(position.x, position.y));
        hitbox.getHitbox().ComputeGlobalVertices();
        hitbox.getHitbox().ComputeGlobalEdges();
        hitbox.getHitbox().ComputeGlobalCenter();
    });
}

}
