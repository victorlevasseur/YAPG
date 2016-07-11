#ifndef YAPG_GAME_COMPONENTS_HEALTHCOMPONENT_H
#define YAPG_GAME_COMPONENTS_HEALTHCOMPONENT_H

#include "Common/Component.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/LuaState.hpp"
#include "Messaging/Messaging.hpp"
#include "Meta/Metadata.hpp"

namespace yapg
{

class HealthComponent : public Component, public messaging::Emitter
{
public:
    HealthComponent(entityx::Entity entity);
    virtual ~HealthComponent();

    virtual std::string getName() const;

    static void registerComponent(LuaState& state);

    void kill();
    void loosePV(float pv);
    void gainPV(float pv);

    float health;
    float maxHealth;
};

std::ostream& operator<<(std::ostream& stream, const HealthComponent& component);

}

#endif
