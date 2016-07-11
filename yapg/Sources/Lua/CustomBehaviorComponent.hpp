#ifndef YAPG_GAME_COMPONENTS_CUSTOMBEHAVIORCOMPONENT_H
#define YAPG_GAME_COMPONENTS_CUSTOMBEHAVIORCOMPONENT_H

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"

namespace components
{

class CustomBehaviorComponent : public Component
{
public:
    CustomBehaviorComponent(entityx::Entity entity);
    virtual ~CustomBehaviorComponent();

    virtual std::string getName() const;

    sol::function updateFunction;

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const CustomBehaviorComponent& component);

}

#endif
