#ifndef YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H
#define YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H

#include <SFML/Graphics/Transform.hpp>

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"

namespace components
{

class PositionComponent : public Component
{
public:
    PositionComponent(entityx::Entity entity);
    virtual ~PositionComponent();

    virtual std::string getName() const;

    sf::Transform getPositionTransform() const;

    float x;
    float y;
    float z;
    float width;
    float height;

    float oldX;
    float oldY;
    float oldWidth;
    float oldHeight;

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const PositionComponent& component);

}

#endif
