#ifndef YAPG_GAME_COMPONENTS_FINISHLINECOMPONENT_H
#define YAPG_GAME_COMPONENTS_FINISHLINECOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class FinishLineComponent : public Component
{
public:
    FinishLineComponent(entityx::Entity entity);
    virtual ~FinishLineComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const FinishLineComponent& component);

}

#endif
