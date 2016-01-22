#ifndef YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H
#define YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"

namespace game
{
namespace components
{

class PositionComponent : public Component
{
public:
    PositionComponent();
    virtual ~PositionComponent();

    virtual std::string getName() const;

    virtual void loadFromLua(const sol::table& luaTable, const level::SerializedEntityGetter& entityGetter);

    float x;
    float y;
    float width;
    float height;

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const PositionComponent& component);

}
}

#endif
