#include "Systems/RenderSystem.hpp"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"

namespace c = game::components;

namespace game
{
namespace systems
{

RenderSystem::RenderSystem(bool debugHitboxDraw) :
    entityx::System<RenderSystem>(),
    m_renderingQueue(),
    m_debugHitboxDraw(debugHitboxDraw)
{

}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    m_renderingQueue.clear();

    /*es.each<c::PositionComponent>([&](entityx::Entity entity, c::PositionComponent& position) {
        auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(position.width, position.height));
        shape->setPosition(position.x, position.y);

        m_renderingQueue.push_back(std::make_pair(shape, sf::RenderStates::Default));
    });*/

    if(m_debugHitboxDraw)
    {
        es.each<c::HitboxComponent>([&](entityx::Entity entity, c::HitboxComponent& hitbox) {
            auto shape = std::make_shared<sf::ConvexShape>(hitbox.getHitbox().GetGlobalVertices().size());
            shape->setOutlineThickness(1.f);
            shape->setFillColor(sf::Color::Transparent);
            shape->setOutlineColor(sf::Color::Black);
            for(unsigned int i = 0; i < shape->getPointCount(); i++)
            {
                shape->setPoint(i, sf::Vector2f(
                    hitbox.getHitbox().GetGlobalVertices()[i].x,
                    hitbox.getHitbox().GetGlobalVertices()[i].y
                ));
            }

            m_renderingQueue.push_back(std::make_pair(shape, sf::RenderStates::Default));
        });
    }
}

void RenderSystem::render(sf::RenderTarget& target)
{
    for(auto it = m_renderingQueue.begin(); it != m_renderingQueue.end(); ++it)
    {
        target.draw(*(it->first), it->second);
    }
}

}
}
