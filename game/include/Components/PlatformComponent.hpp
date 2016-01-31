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
    PlatformComponent();
    virtual ~PlatformComponent();

    virtual std::string getName() const;

    virtual void loadFromLua(const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter);

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const PlatformComponent& component);

}
}

#endif
