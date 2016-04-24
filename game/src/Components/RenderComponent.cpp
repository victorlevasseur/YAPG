#include "Components/RenderComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

RenderComponent::RenderComponent() :
    Component(),
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

    lua::EntityHandle::declareComponent<RenderComponent>("Render");

    state.getState().new_usertype<RenderComponent>("render_component",
        "texture", &RenderComponent::textureName,
        "current_animation", &RenderComponent::currentAnimation,
        "animations", &RenderComponent::animations,
        "flipped", &RenderComponent::flipped,
        "on_animation_changed", &RenderComponent::onAnimationChangedFunc,
        "on_animation_end", &RenderComponent::onAnimationEndFunc
    );
    state.declareComponentGetter<RenderComponent>("render");
}

std::ostream& operator<<(std::ostream& stream, const RenderComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
