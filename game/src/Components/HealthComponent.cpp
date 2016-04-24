#include "Components/HealthComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

HealthComponent::HealthComponent() :
    Component()
{

}

HealthComponent::~HealthComponent()
{

}

std::string HealthComponent::getName() const
{
    return "Health";
}

void HealthComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<HealthComponent>()
        .declareAttribute("health", &HealthComponent::health)
        .declareAttribute("max_health", &HealthComponent::maxHealth);

    lua::EntityHandle::declareComponent<HealthComponent>("Health");

    state.getState().new_usertype<HealthComponent>("health_component",
        "health", &HealthComponent::health,
        "max_health", &HealthComponent::maxHealth
    );
    state.declareComponentGetter<HealthComponent>("health");
}

std::ostream& operator<<(std::ostream& stream, const HealthComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
