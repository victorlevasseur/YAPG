#ifndef YAPG_GAME_COMPONENTS_HEALTHCOMPONENT_H
#define YAPG_GAME_COMPONENTS_HEALTHCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/LuaState.hpp"
#include "Messaging/Messaging.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class HealthComponent : public Component, public messaging::Emitter
{
public:
    HealthComponent();
    virtual ~HealthComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    static void kill(lua::EntityHandle entity);
    static void loosePV(lua::EntityHandle entity, float pv);

    float health;
    float maxHealth;
};

std::ostream& operator<<(std::ostream& stream, const HealthComponent& component);

}

#endif
