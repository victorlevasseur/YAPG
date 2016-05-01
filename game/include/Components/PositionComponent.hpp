#ifndef YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H
#define YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"


namespace components
{

class PositionComponent : public Component
{
public:
    PositionComponent();
    virtual ~PositionComponent();

    virtual std::string getName() const;

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
