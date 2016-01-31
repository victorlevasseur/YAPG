#ifndef YAPG_GAME_COMPONENTS_PLATFORMERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLATFORMERCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"

namespace game
{
namespace components
{

class PlatformerComponent : public Component
{
public:
    PlatformerComponent();
    virtual ~PlatformerComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const PlatformerComponent& component);

}
}

#endif
