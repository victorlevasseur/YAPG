#include "Components/ColliderComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

ColliderComponent::ColliderComponent() :
    Component()
{

}

ColliderComponent::~ColliderComponent()
{

}

std::string ColliderComponent::getName() const
{
    return "Collider";
}

void ColliderComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<ColliderComponent>();

    lua::EntityHandle::declareComponent<ColliderComponent>("collider");

    state.getState().new_usertype<ColliderComponent>("collider_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
    state.declareComponentGetter<ColliderComponent>("collider");
}

std::ostream& operator<<(std::ostream& stream, const ColliderComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
