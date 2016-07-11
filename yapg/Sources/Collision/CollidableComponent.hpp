#ifndef YAPG_GAME_COMPONENTS_COLLIDABLECOMPONENT_H
#define YAPG_GAME_COMPONENTS_COLLIDABLECOMPONENT_H

#include <map>

#include "Collision/PolygonCallback.hpp"
#include "Common/Component.hpp"
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

    std::map<std::string, collision::PolygonCallback> polygons;
    std::vector<std::pair<std::string, collision::PolygonCallback>> polygonsByPriority; ///< Store pointers to the polygons ordered by priority (currently, exclusive polygons are first)
};

std::ostream& operator<<(std::ostream& stream, const CollidableComponent& component);

}

#endif
