#ifndef YAPG_GAME_COMPONENTS_COLLIDERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_COLLIDERCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class ColliderComponent : public Component
{
public:
    ColliderComponent(entityx::Entity entity);
    virtual ~ColliderComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const ColliderComponent& component);

}

#endif
