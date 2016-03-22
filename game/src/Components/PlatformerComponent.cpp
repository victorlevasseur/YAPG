#include "Components/PlatformerComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

PlatformerComponent::PlatformerComponent() :
    Component(),
    wantsToJump(false),
    wantsToGoLeft(false),
    wantsToGoRight(false),
    layer(0),
    maxFallingSpeed(400.f),
    maxJumpingSpeed(600.f),
    currentSpeed(0),
    fallingSpeed(0),
    jumpingSpeed(0),
    acceleration(1000.f),
    deceleration(1000.f),
    maxSpeed(170.f),
    gravity(700.f),
    canJumpAgain(false),
    groundEntity(),
    oldFloorPosX(0),
    oldFloorPosY(0),
    movementStateCallbacks(Idle),
    directionStateCallbacks(Right)
{

}

PlatformerComponent::~PlatformerComponent()
{

}

std::string PlatformerComponent::getName() const
{
    return "Platformer";
}

void PlatformerComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<PlatformerComponent>("PlatformerComponent")
        .declareAttribute("onIdle", &PlatformerComponent::onIdleFunc)
        .declareAttribute("onStartWalking", &PlatformerComponent::onWalkingFunc)
        .declareAttribute("onStartJumping", &PlatformerComponent::onJumpingFunc)
        .declareAttribute("onStartFalling", &PlatformerComponent::onFallingFunc)
        .declareAttribute("onTurnRight", &PlatformerComponent::onTurnRightFunc)
        .declareAttribute("onTurnLeft", &PlatformerComponent::onTurnLeftFunc)
        .declareAttribute("max_walking_speed", &PlatformerComponent::maxSpeed)
        .declareAttribute("max_jumping_speed", &PlatformerComponent::maxJumpingSpeed)
        .declareAttribute("max_falling_speed", &PlatformerComponent::maxFallingSpeed)
        .declareAttribute("acceleration", &PlatformerComponent::acceleration)
        .declareAttribute("deceleration", &PlatformerComponent::deceleration)
        .declareAttribute("wants_to_jump", &PlatformerComponent::wantsToJump)
        .declareAttribute("wants_to_go_left", &PlatformerComponent::wantsToGoLeft)
        .declareAttribute("wants_to_go_right", &PlatformerComponent::wantsToGoRight)
        .setExtraLoadFunction([](PlatformerComponent* c, const sol::object& luaObject) {
            c->movementStateCallbacks.registerCallback(State::Idle, c->onIdleFunc);
            c->movementStateCallbacks.registerCallback(State::Walking, c->onWalkingFunc);
            c->movementStateCallbacks.registerCallback(State::Jumping, c->onJumpingFunc);
            c->movementStateCallbacks.registerCallback(State::Falling, c->onFallingFunc);

            c->directionStateCallbacks.registerCallback(Direction::Right, c->onTurnRightFunc);
            c->directionStateCallbacks.registerCallback(Direction::Left, c->onTurnLeftFunc);
        });

    lua::EntityHandle::declareComponent<PlatformerComponent>("Platformer");

    state.getState().new_usertype<PlatformerComponent>("platformer_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const PlatformerComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
