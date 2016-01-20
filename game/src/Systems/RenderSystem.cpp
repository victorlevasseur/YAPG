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
    m_renderingQueue()
{

}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    m_renderingQueue.clear();

    es.each<c::PositionComponent>([&](entityx::Entity entity, c::PositionComponent& position) {
        auto shape = std::make_shared<sf::RectangleShape>(sf::Vector2f(position.width, position.height));
        shape->setPosition(position.x, position.y);

        m_renderingQueue.push_back(std::make_pair(shape, sf::RenderStates::Default));
    });
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
