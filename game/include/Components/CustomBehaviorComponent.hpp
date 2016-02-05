#ifndef YAPG_GAME_COMPONENTS_CUSTOMBEHAVIORCOMPONENT_H
#define YAPG_GAME_COMPONENTS_CUSTOMBEHAVIORCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"

namespace components
{

class CustomBehaviorComponent : public Component
{
public:
    CustomBehaviorComponent();
    virtual ~CustomBehaviorComponent();

    virtual std::string getName() const;

    sol::function updateFunction;

    static void registerComponent(lua::LuaState& state);

    DECLARE_COMPONENT(CustomBehaviorComponent)
};

std::ostream& operator<<(std::ostream& stream, const CustomBehaviorComponent& component);

}

#endif
