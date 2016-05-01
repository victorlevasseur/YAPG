#include "Systems/RenderSystem.hpp"

#include <algorithm>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Components/HitboxComponent.hpp"
#include "Components/PlatformerComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Lua/EntityHandle.hpp"

namespace c = components;


namespace systems
{

RenderSystem::RenderSystem(resources::TexturesManager& texturesManager, tools::EntitySpatialGrid& grid, bool cameraFollowPlayers, bool debugHitboxDraw) :
    entityx::System<RenderSystem>(),
    m_cameraFollowPlayers(cameraFollowPlayers),
    m_renderingQueue(),
    m_viewInit(false),
    m_renderingView(),
    m_centerOnPlayer(0),
    m_lastGroundEntity(),
    m_animatedSprites(),
    m_texturesManager(texturesManager),
    m_debugHitboxDraw(debugHitboxDraw),
    m_grid(grid)
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

    auto drawFunc = [&](entityx::Entity entity, c::PositionComponent& position, c::RenderComponent& render) {
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

            if(render.onAnimationChangedFunc.valid())
                render.onAnimationChangedFunc.call(lua::EntityHandle(entity), oldAnimation, render.currentAnimation);
        }

        animatedSprite->update(dt);
        animatedSprite->setOrigin(sf::Vector2f(0.5f, 0.5f));
        animatedSprite->setPosition(position.x + position.width/2.f, position.y + position.height/2.f);
        animatedSprite->setScale((render.flipped ? (-1) : (1)) * position.width, position.height);

        //Call the lua callback if the animation has just been restarted
        if(animatedSprite->hadRestartedAnimation() && render.onAnimationEndFunc.valid())
            render.onAnimationEndFunc.call(lua::EntityHandle(entity), render.currentAnimation);

        addToRenderingQueue(animatedSprite, sf::RenderStates::Default, position.z); //TODO: Get z position from RenderComponent
    };

    sf::FloatRect viewAABB(
        m_renderingView.getCenter().x - 1024.f/2.f,
        m_renderingView.getCenter().y - 768.f/2.f,
        1024.f,
        768.f
    );

    std::set<entityx::Entity> entitiesToDraw = m_grid.getEntitiesIntersectingAABB(viewAABB);
    for(entityx::Entity entity : entitiesToDraw)
    {
        if(entity.has_component<c::PositionComponent>() && entity.has_component<c::RenderComponent>())
        {
            drawFunc(entity, *(entity.component<c::PositionComponent>().get()), *(entity.component<c::RenderComponent>().get()));
        }
    }

    if(m_cameraFollowPlayers)
    {
        //Update the camera
        es.each<c::PlayerComponent, c::PlatformerComponent, c::PositionComponent>([&](entityx::Entity entity, c::PlayerComponent& player, c::PlatformerComponent& platformer, c::PositionComponent& position) {
            if(player.playerNumber != m_centerOnPlayer)
                return; //Only center on the selected player

            float newX = position.x + position.width / 2.f;
            float newY = m_renderingView.getCenter().y;

            if(platformer.groundEntity && platformer.groundEntity.has_component<c::PositionComponent>())
            {
                m_lastGroundEntity = platformer.groundEntity;
            }

            if(m_lastGroundEntity)
            {
                auto groundPosition = m_lastGroundEntity.component<c::PositionComponent>();

                //If the player is on the ground, update to Y position.
                if(std::abs(newY - groundPosition->y) > 10.f)
                {
                    newY += (newY > groundPosition->y ? -1 : 1) * 150.f * dt;
                }

                //If the player goes beyond some limits, remove the last ground entity and try to center on player
                if(position.y + position.height > newY + 200.f || position.y + position.height < newY - 200.f)
                {
                    m_lastGroundEntity = entityx::Entity();
                }
            }
            else
            {
                newY += (newY > position.y ? -1 : 1) * std::abs(newY - position.y) * 2 * dt + (platformer.fallingSpeed - platformer.jumpingSpeed) * dt;
            }

            m_renderingView.setCenter(sf::Vector2f(newX, newY));
        });
    }
}

void RenderSystem::render(sf::RenderTarget& target)
{
    if(!m_viewInit)
    {
        m_renderingView = target.getView();
        m_viewInit = true;
    }

    sf::View oldView = target.getView();

    target.setView(m_renderingView);

    for(auto it = m_renderingQueue.cbegin(); it != m_renderingQueue.cend(); ++it)
    {
        target.draw(*(it->drawable), it->states);
    }

    target.setView(oldView);
}

void RenderSystem::addToRenderingQueue(std::shared_ptr<sf::Drawable> drawable, sf::RenderStates states, float z)
{
    auto insertionIt = std::lower_bound(m_renderingQueue.begin(), m_renderingQueue.end(), z, [](Renderable& renderable, float z) { return renderable.z < z; });
    m_renderingQueue.insert(insertionIt, Renderable{drawable, states, z});
}

}
