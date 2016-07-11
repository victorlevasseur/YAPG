#include "Health/HealthComponent.hpp"

#include "Health/HealthSystem.hpp"

namespace yapg
{

HealthComponent::HealthComponent(entityx::Entity entity) :
    Component(entity)
{

}

HealthComponent::~HealthComponent()
{

}

std::string HealthComponent::getName() const
{
    return "Health";
}

void HealthComponent::registerComponent(LuaState& state)
{
    meta::MetadataStore::registerClass<HealthComponent>()
        .declareAttribute("health", &HealthComponent::health)
        .declareAttribute("max_health", &HealthComponent::maxHealth);

    EntityHandle::declareComponent<HealthComponent>("health");

    state.getState().new_usertype<HealthComponent>("health_component",
        "health", sol::readonly(&HealthComponent::health),
        "max_health", &HealthComponent::maxHealth,
        "kill", &HealthComponent::kill,
        "loose_pv", &HealthComponent::loosePV,
        "gain_pv", &HealthComponent::gainPV
    );
    state.declareComponentGetter<HealthComponent>("health");
}

void HealthComponent::kill()
{
    emit<HealthKillMessage>(getEntity());
}

void HealthComponent::loosePV(float pv)
{
    emit<HealthLoosePVMessage>(getEntity(), pv);
}

void HealthComponent::gainPV(float pv)
{
    emit<HealthGainPVMessage>(getEntity(), pv);
}

std::ostream& operator<<(std::ostream& stream, const HealthComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
