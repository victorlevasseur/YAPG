#ifndef YAPG_GAME_COMPONENTS_PLATFORMERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLATFORMERCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"

namespace game
{
namespace components
{

class PlatformerComponent : public Component
{
public:
    PlatformerComponent();
    virtual ~PlatformerComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    int layer;

    float maxFallingSpeed;
    float maxJumpingSpeed;

    float currentSpeed;
    float fallingSpeed;
    float jumpingSpeed;

    float acceleration;
    float deceleration;
    float maxSpeed;

    float gravity;
    bool canJumpAgain;

    entityx::Entity groundEntity;
    float oldFloorPosX;
    float oldFloorPosY;
};

std::ostream& operator<<(std::ostream& stream, const PlatformerComponent& component);

}
}

#endif
