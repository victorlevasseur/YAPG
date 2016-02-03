#ifndef YAPG_GAME_COMPONENTS_RENDERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_RENDERCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"


namespace components
{

class RenderComponent : public Component
{
public:
    RenderComponent();
    virtual ~RenderComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const RenderComponent& component);

}

#endif
