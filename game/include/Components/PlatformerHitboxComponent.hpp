#ifndef YAPG_GAME_COMPONENTS_HITBOXCOMPONENT_H
#define YAPG_GAME_COMPONENTS_HITBOXCOMPONENT_H

#include "Collision/Polygon.hpp"
#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class PlatformerHitboxComponent : public Component
{
public:
    PlatformerHitboxComponent(entityx::Entity entity);
    virtual ~PlatformerHitboxComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    collision::Polygon& getHitbox() { return m_polygon; }
    const collision::Polygon& getHitbox() const { return m_polygon; }

private:
    collision::Polygon m_polygon;
};

std::ostream& operator<<(std::ostream& stream, const PlatformerHitboxComponent& component);

}

#endif
