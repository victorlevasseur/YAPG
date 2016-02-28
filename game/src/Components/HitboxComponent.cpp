#include "Components/HitboxComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

HitboxComponent::HitboxComponent() :
    Component()
{

}

HitboxComponent::~HitboxComponent()
{

}

std::string HitboxComponent::getName() const
{
    return "Hitbox";
}

void HitboxComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<HitboxComponent>("Hitbox")
        .declareAttribute("polygon", &HitboxComponent::m_polygon);

    lua::EntityHandle::declareComponent<HitboxComponent>("Hitbox");

    state.getState().new_usertype<HitboxComponent>("hitbox_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const HitboxComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
