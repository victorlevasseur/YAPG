#ifndef YAPG_GAME_SYSTEMS_RENDERSYSTEM_H
#define YAPG_GAME_SYSTEMS_RENDERSYSTEM_H

#include <SFML/Graphics/RenderTarget.hpp>

#include "entityx/entityx.h"

namespace game
{
namespace systems
{

class RenderSystem : public entityx::System<RenderSystem>
{
public:
    RenderSystem();

    sf::RenderTarget* getRenderTarget() const;
    void setRenderTarget(sf::RenderTarget* target);

    virtual void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
    sf::RenderTarget* m_target;
};

}
}

#endif
