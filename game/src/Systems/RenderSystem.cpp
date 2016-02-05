#include "Systems/RenderSystem.hpp"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"

namespace c = components;


namespace systems
{

RenderSystem::RenderSystem(resources::TexturesManager& texturesManager, bool debugHitboxDraw) :
    entityx::System<RenderSystem>(),
    m_renderingQueue(),
    m_animatedSprites(),
    m_texturesManager(texturesManager),
    m_debugHitboxDraw(debugHitboxDraw)
{

}

void RenderSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    m_renderingQueue.clear();

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

    es.each<c::PositionComponent, c::RenderComponent>([&](entityx::Entity entity, c::PositionComponent& position, c::RenderComponent& render) {
        if(m_animatedSprites.count(entity) == 0)
        {
            //Create the animated sprite if it doesn't exist
            auto animatedSprite = std::make_shared<animation::AnimatedSprite>(
                m_texturesManager.requestResource(render.textureName),
                render.animations
            );
            animatedSprite->setCurrentAnimation(render.currentAnimation);
            m_animatedSprites[entity] = animatedSprite;
        }

        //Update the animated sprite and put it in the render queue
        auto animatedSprite = m_animatedSprites[entity];
        if(render.currentAnimation != animatedSprite->getCurrentAnimation())
            animatedSprite->setCurrentAnimation(render.currentAnimation);

        animatedSprite->update(dt);
        animatedSprite->setPosition(position.x, position.y);
        animatedSprite->setScale(position.width, position.height);

        m_renderingQueue.push_back(std::make_pair(animatedSprite, sf::RenderStates::Default));
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
