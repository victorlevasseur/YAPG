#ifndef YAPG_GAME_COMPONENTS_HITBOXCOMPONENT_H
#define YAPG_GAME_COMPONENTS_HITBOXCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"
#include "Tools/Polygon.hpp"

namespace game
{
namespace components
{

class HitboxComponent : public Component
{
public:
    HitboxComponent();
    virtual ~HitboxComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    tools::Polygon& getHitbox() { return m_polygon; }
    const tools::Polygon& getHitbox() const { return m_polygon; }

private:
    tools::Polygon m_polygon;
};

std::ostream& operator<<(std::ostream& stream, const HitboxComponent& component);

}
}

#endif
