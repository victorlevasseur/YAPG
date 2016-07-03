#include "Components/RenderComponent.hpp"

#include "Lua/EntityHandle.hpp"
#include "Systems/RenderSystem.hpp"

namespace components
{

RenderComponent::RenderComponent(entityx::Entity entity) :
    Component(entity),
    textureName(),
    currentAnimation(),
    animations(),
    flipped(false)
{

}

RenderComponent::~RenderComponent()
{

}

std::string RenderComponent::getName() const
{
    return "Render";
}

void RenderComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<RenderComponent>("RenderComponent")
        .declareAttribute("texture", &RenderComponent::textureName)
        .declareAttribute("current_animation", &RenderComponent::currentAnimation)
        .declareAttribute("animations", &RenderComponent::animations)
        .declareAttribute("flipped", &RenderComponent::flipped)
        .declareAttribute("on_animation_changed", &RenderComponent::onAnimationChangedFunc)
        .declareAttribute("on_animation_end", &RenderComponent::onAnimationEndFunc);

    lua::EntityHandle::declareComponent<RenderComponent>("render");

    state.getState().new_usertype<RenderComponent>("render_component",
        "texture", &RenderComponent::textureName,
        "current_animation", sol::property(&RenderComponent::getCurrentAnimation, &RenderComponent::setCurrentAnimation),
        "animations", &RenderComponent::animations,
        "flipped", &RenderComponent::flipped,
        "on_animation_changed", &RenderComponent::onAnimationChangedFunc,
        "on_animation_end", &RenderComponent::onAnimationEndFunc
    );
    state.declareComponentGetter<RenderComponent>("render");
}

void RenderComponent::setCurrentAnimation(const std::string& animationName)
{
    currentAnimation = animationName;
    emit<systems::AnimationChangedMessage>(getEntity());
}

std::ostream& operator<<(std::ostream& stream, const RenderComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
