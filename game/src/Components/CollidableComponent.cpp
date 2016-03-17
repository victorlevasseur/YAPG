#include "Components/CollidableComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

CollidableComponent::CollidableComponent() :
    Component()
{

}

CollidableComponent::~CollidableComponent()
{

}

std::string CollidableComponent::getName() const
{
    return "Collidable";
}

void CollidableComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<CollidableComponent>()
        .declareAttribute("on_collision_begin", &CollidableComponent::onCollisionBegin)
        .declareAttribute("collides", &CollidableComponent::collides)
        .declareAttribute("on_collision_end", &CollidableComponent::onCollisionEnd);

    lua::EntityHandle::declareComponent<CollidableComponent>("Collidable");

    state.getState().new_usertype<CollidableComponent>("collidable_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const CollidableComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
