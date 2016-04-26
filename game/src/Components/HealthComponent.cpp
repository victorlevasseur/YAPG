#include "Components/HealthComponent.hpp"

#include "Systems/HealthSystem.hpp"

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

    lua::EntityHandle::declareComponent<HealthComponent>("health");

    state.getState().new_usertype<HealthComponent>("health_component",
        "health", sol::readonly(&HealthComponent::health),
        "max_health", &HealthComponent::maxHealth,
        "kill", &HealthComponent::kill
    );
    state.declareComponentGetter<HealthComponent>("health");
}

void HealthComponent::kill(lua::EntityHandle entity)
{
    emit<systems::HealthKillMessage>(entity);
}

std::ostream& operator<<(std::ostream& stream, const HealthComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
