#ifndef YAPG_GAME_COMPONENTS_PLATFORMERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLATFORMERCOMPONENT_H

#include "Common/Component.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/LuaState.hpp"
#include "Lua/EnumStateCallbacks.hpp"

namespace yapg
{

class PlatformerComponent : public Component
{
public:
    enum State
    {
        Idle,
        Walking,
        Jumping,
        Falling
    };

    enum Direction
    {
        Right,
        Left
    };

    PlatformerComponent(entityx::Entity entity);
    virtual ~PlatformerComponent();

    virtual std::string getName() const;

    static void registerComponent(LuaState& state);

    bool wantsToJump;
    bool wantsToGoLeft;
    bool wantsToGoRight;

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

    EnumStateCallbacks<4, State, EntityHandle> movementStateCallbacks;
    sol::function onIdleFunc;
    sol::function onWalkingFunc;
    sol::function onJumpingFunc;
    sol::function onFallingFunc;

    EnumStateCallbacks<2, Direction, EntityHandle> directionStateCallbacks;
    sol::function onTurnRightFunc;
    sol::function onTurnLeftFunc;
};

std::ostream& operator<<(std::ostream& stream, const PlatformerComponent& component);

}

#endif
