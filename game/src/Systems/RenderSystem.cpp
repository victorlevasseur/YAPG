#include "Systems/RenderSystem.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

#include "Components/PositionComponent.hpp"

namespace c = game::components;

namespace game
{
namespace systems
{

RenderSystem::RenderSystem() :
    entityx::System<RenderSystem>(),
    m_target(nullptr)
{

}

sf::RenderTarget* RenderSystem::getRenderTarget() const
{
    return m_target;
}

void RenderSystem::setRenderTarget(sf::RenderTarget* target)
{
    m_target = target;
}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    if(m_target == nullptr)
        return;

    es.each<c::PositionComponent>([&](entityx::Entity entity, c::PositionComponent& position) {
        sf::RectangleShape shape(sf::Vector2f(position.width, position.height));
        shape.setPosition(position.x, position.y);

        m_target->draw(shape);
    });
}

}
}
