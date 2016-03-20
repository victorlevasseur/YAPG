#ifndef YAPG_GAME_SYSTEMS_RENDERSYSTEM_H
#define YAPG_GAME_SYSTEMS_RENDERSYSTEM_H

#include <list>
#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "entityx/entityx.h"

#include "Animation/AnimatedSprite.hpp"
#include "Resources/ResourcesManager.hpp"

namespace systems
{

class RenderSystem : public entityx::System<RenderSystem>
{
public:
    struct Renderable
    {
        std::shared_ptr<sf::Drawable> drawable;
        sf::RenderStates states;
        float z;
    };

    RenderSystem(resources::TexturesManager& texturesManager, bool disableViewManage = false, bool debugHitboxDraw = false);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    void render(sf::RenderTarget& target);

private:
    void addToRenderingQueue(std::shared_ptr<sf::Drawable> drawable, sf::RenderStates states, float z);

    bool m_noViewManage;

    std::list<Renderable> m_renderingQueue;
    bool m_viewInit;
    sf::View m_renderingView;
    int m_centerOnPlayer;
    entityx::Entity m_lastGroundEntity;

    std::map<entityx::Entity, std::shared_ptr<animation::AnimatedSprite>> m_animatedSprites;

    resources::TexturesManager& m_texturesManager;
    bool m_debugHitboxDraw;
};

}

#endif
