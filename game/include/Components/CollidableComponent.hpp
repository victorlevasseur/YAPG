#ifndef YAPG_GAME_COMPONENTS_COLLIDABLECOMPONENT_H
#define YAPG_GAME_COMPONENTS_COLLIDABLECOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class CollidableComponent : public Component
{
public:
    CollidableComponent(entityx::Entity entity);
    virtual ~CollidableComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    sol::function onCollisionBegin;
    sol::function collides;
    sol::function onCollisionEnd;
};

std::ostream& operator<<(std::ostream& stream, const CollidableComponent& component);

}

#endif
