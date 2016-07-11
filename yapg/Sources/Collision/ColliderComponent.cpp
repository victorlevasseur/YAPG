#include "Collision/ColliderComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace yapg
{

ColliderComponent::ColliderComponent(entityx::Entity entity) :
    Component(entity)
{

}

ColliderComponent::~ColliderComponent()
{

}

std::string ColliderComponent::getName() const
{
    return "Collider";
}

void ColliderComponent::registerComponent(LuaState& state)
{
    meta::MetadataStore::registerClass<ColliderComponent>();

    EntityHandle::declareComponent<ColliderComponent>("collider");

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
