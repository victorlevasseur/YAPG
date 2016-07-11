#include "Platformer/PlatformerComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace yapg
{

PlatformerComponent::PlatformerComponent(entityx::Entity entity) :
    Component(entity),
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

void PlatformerComponent::registerComponent(LuaState& state)
{
    meta::MetadataStore::registerClass<PlatformerComponent>("PlatformerComponent")
        .declareAttribute("max_walking_speed", &PlatformerComponent::maxSpeed)
        .declareAttribute("max_jumping_speed", &PlatformerComponent::maxJumpingSpeed)
        .declareAttribute("max_falling_speed", &PlatformerComponent::maxFallingSpeed)
        .declareAttribute("acceleration", &PlatformerComponent::acceleration)
        .declareAttribute("deceleration", &PlatformerComponent::deceleration)
        .declareAttribute("gravity", &PlatformerComponent::gravity)
        .declareAttribute("wants_to_jump", &PlatformerComponent::wantsToJump)
        .declareAttribute("wants_to_go_left", &PlatformerComponent::wantsToGoLeft)
        .declareAttribute("wants_to_go_right", &PlatformerComponent::wantsToGoRight)
        .declareAttribute("on_idle", &PlatformerComponent::onIdleFunc)
        .declareAttribute("on_start_walking", &PlatformerComponent::onWalkingFunc)
        .declareAttribute("on_start_jumping", &PlatformerComponent::onJumpingFunc)
        .declareAttribute("on_start_falling", &PlatformerComponent::onFallingFunc)
        .declareAttribute("on_turn_right", &PlatformerComponent::onTurnRightFunc)
        .declareAttribute("on_turn_left", &PlatformerComponent::onTurnLeftFunc)
        .setExtraLoadFunction([](PlatformerComponent* c, const sol::object& luaObject) {
            c->movementStateCallbacks.registerCallback(State::Idle, c->onIdleFunc);
            c->movementStateCallbacks.registerCallback(State::Walking, c->onWalkingFunc);
            c->movementStateCallbacks.registerCallback(State::Jumping, c->onJumpingFunc);
            c->movementStateCallbacks.registerCallback(State::Falling, c->onFallingFunc);

            c->directionStateCallbacks.registerCallback(Direction::Right, c->onTurnRightFunc);
            c->directionStateCallbacks.registerCallback(Direction::Left, c->onTurnLeftFunc);
        });

    EntityHandle::declareComponent<PlatformerComponent>("platformer");

    state.getState().new_usertype<PlatformerComponent>("platformer_component",
        "max_walking_speed", &PlatformerComponent::maxSpeed,
        "max_jumping_speed", &PlatformerComponent::maxJumpingSpeed,
        "max_falling_speed", &PlatformerComponent::maxFallingSpeed,
        "acceleration", &PlatformerComponent::acceleration,
        "deceleration", &PlatformerComponent::deceleration,
        "gravity", &PlatformerComponent::gravity,
        "wants_to_jump", &PlatformerComponent::wantsToJump,
        "wants_to_go_left", &PlatformerComponent::wantsToGoLeft,
        "wants_to_go_right", &PlatformerComponent::wantsToGoRight,
        "on_idle", &PlatformerComponent::onIdleFunc,
        "on_start_walking", &PlatformerComponent::onWalkingFunc,
        "on_start_jumping", &PlatformerComponent::onJumpingFunc,
        "on_start_falling", &PlatformerComponent::onFallingFunc,
        "on_turn_right", &PlatformerComponent::onTurnRightFunc,
        "on_turn_left", &PlatformerComponent::onTurnLeftFunc
    );
    state.declareComponentGetter<PlatformerComponent>("platformer");
}

std::ostream& operator<<(std::ostream& stream, const PlatformerComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
