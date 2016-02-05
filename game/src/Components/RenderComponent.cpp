#include "Components/RenderComponent.hpp"


namespace components
{

RenderComponent::RenderComponent() :
    Component(),
    textureName(),
    currentAnimation(),
    animations()
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
    meta::MetadataStore::registerClass<RenderComponent>()
        .declareAttribute("texture", &RenderComponent::textureName)
        .declareAttribute("default_animation", &RenderComponent::currentAnimation)
        .declareAttribute("animations", &RenderComponent::animations);

    state.getState().new_usertype<RenderComponent>("render_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const RenderComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
