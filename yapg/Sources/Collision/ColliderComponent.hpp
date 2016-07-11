#ifndef YAPG_GAME_COMPONENTS_COLLIDERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_COLLIDERCOMPONENT_H

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace yapg
{

class ColliderComponent : public Component
{
public:
    ColliderComponent(entityx::Entity entity);
    virtual ~ColliderComponent();

    virtual std::string getName() const;

    static void registerComponent(LuaState& state);
};

std::ostream& operator<<(std::ostream& stream, const ColliderComponent& component);

}

#endif
