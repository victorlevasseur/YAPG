#include "Systems/RenderSystem.hpp"

#include <algorithm>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Lua/EntityHandle.hpp"

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

            addToRenderingQueue(shape, sf::RenderStates::Default, 100000.f);
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
        {
            std::string oldAnimation = animatedSprite->getCurrentAnimation();
            animatedSprite->setCurrentAnimation(render.currentAnimation);

            if(render.onAnimationChangedFunc.state())
                render.onAnimationChangedFunc.call(lua::EntityHandle(entity), oldAnimation, render.currentAnimation);
        }

        animatedSprite->update(dt);
        animatedSprite->setOrigin(sf::Vector2f(0.5f, 0.5f));
        animatedSprite->setPosition(position.x + position.width/2.f, position.y + position.height/2.f);
        animatedSprite->setScale((render.flipped ? (-1) : (1)) * position.width, position.height);

        //Call the lua callback if the animation has just been restarted
        if(animatedSprite->hadRestartedAnimation() && render.onAnimationEndFunc.state())
            render.onAnimationEndFunc.call(lua::EntityHandle(entity), render.currentAnimation);

        addToRenderingQueue(animatedSprite, sf::RenderStates::Default, position.z); //TODO: Get z position from RenderComponent
    });
}

void RenderSystem::render(sf::RenderTarget& target)
{
    for(auto it = m_renderingQueue.cbegin(); it != m_renderingQueue.cend(); ++it)
    {
        target.draw(*(it->drawable), it->states);
    }
}

void RenderSystem::addToRenderingQueue(std::shared_ptr<sf::Drawable> drawable, sf::RenderStates states, float z)
{
    auto insertionIt = std::lower_bound(m_renderingQueue.begin(), m_renderingQueue.end(), z, [](Renderable& renderable, float z) { return renderable.z < z; });
    m_renderingQueue.insert(insertionIt, Renderable{drawable, states, z});
}

}
