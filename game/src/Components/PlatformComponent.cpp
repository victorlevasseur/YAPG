#include "Components/PlatformComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

PlatformComponent::PlatformComponent() :
    Component(),
    platformType(Platform),
    layer(0)
{

}

PlatformComponent::~PlatformComponent()
{

}

std::string PlatformComponent::getName() const
{
    return "Platform";
}

void PlatformComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<PlatformComponent>();

    lua::EntityHandle::declareComponent<PlatformComponent>("Platform");

    state.getState().new_usertype<PlatformComponent>("platform_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const PlatformComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
