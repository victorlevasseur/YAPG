#ifndef YAPG_GAME_SYSTEMS_RENDERSYSTEM_H
#define YAPG_GAME_SYSTEMS_RENDERSYSTEM_H

#include <list>
#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "entityx/entityx.h"

#include "Animation/AnimatedSprite.hpp"
#include "Collision/EntitySpatialGrid.hpp"
#include "Messaging/Messaging.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Systems/HitboxUpdaterSystem.hpp"

namespace systems
{

struct ChangeAnimationMessage
{
    entityx::Entity entity;
    std::string animationName;
};

class RenderSystem : public entityx::System<RenderSystem>, public messaging::Receiver<ChangeAnimationMessage>
{
public:
    struct Renderable
    {
        std::shared_ptr<sf::Drawable> drawable;
        sf::RenderStates states;
        float z;
    };

    RenderSystem(resources::TexturesManager& texturesManager, collision::EntitySpatialGrid& grid, bool cameraFollowPlayers = true, bool debugHitboxDraw = false);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    void render(sf::RenderTarget& target);

    virtual void receive(const ChangeAnimationMessage& msg);

    sf::View getView() const { return m_renderingView; }
    void setView(sf::View view) { m_renderingView = view; m_viewInit = true; }

private:
    void addToRenderingQueue(std::shared_ptr<sf::Drawable> drawable, sf::RenderStates states, float z);

    std::shared_ptr<animation::AnimatedSprite> getAnimatedSprite(entityx::Entity entity);

    bool m_cameraFollowPlayers;

    std::list<Renderable> m_renderingQueue;
    bool m_viewInit;
    sf::View m_renderingView;
    int m_centerOnPlayer;
    entityx::Entity m_lastGroundEntity;

    std::map<entityx::Entity, std::shared_ptr<animation::AnimatedSprite>> m_animatedSprites;

    resources::TexturesManager& m_texturesManager;
    bool m_debugHitboxDraw;

    collision::EntitySpatialGrid& m_grid;
};

}

#endif
