#ifndef YAPG_GAME_COMPONENTS_PLATFORMCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLATFORMCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"

namespace game
{
namespace components
{

class PlatformComponent : public Component
{
public:
    enum PlatformType
    {
        Platform = 1,
        Jumpthru = 2,
        All = Platform|Jumpthru
    };

    PlatformComponent();
    virtual ~PlatformComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    PlatformType platformType;
    int layer;
};

std::ostream& operator<<(std::ostream& stream, const PlatformComponent& component);

}
}

#endif
