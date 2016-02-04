#ifndef YAPG_GAME_SYSTEMS_RENDERSYSTEM_H
#define YAPG_GAME_SYSTEMS_RENDERSYSTEM_H

#include <list>
#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "entityx/entityx.h"

#include "Resources/ResourcesManager.hpp"

namespace systems
{

class RenderSystem : public entityx::System<RenderSystem>
{
public:
    RenderSystem(resources::TexturesManager& texturesManager, bool debugHitboxDraw = false);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

    void render(sf::RenderTarget& target);

private:
    std::list<std::pair<std::shared_ptr<sf::Drawable>, sf::RenderStates>> m_renderingQueue;

    resources::TexturesManager& m_texturesManager;
    bool m_debugHitboxDraw;
};

}

#endif
