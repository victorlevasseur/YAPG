#ifndef YAPG_GAME_COMPONENTS_COMPONENT_H
#define YAPG_GAME_COMPONENTS_COMPONENT_H

#include <map>
#include <string>

#include "entityx/entityx.h"

#include "Level/SerializedEntityGetter.hpp"
#include "Lua/sol.hpp"
#include "Meta/Metadata.hpp"

namespace lua{ class LuaState; }

namespace components
{

class Component
{
public:
    Component();
    virtual ~Component();

    /**
     * Return the name of the component
     */
    virtual std::string getName() const { return ""; }

    /**
     * Load the component from its lua table.
     */
    void loadFromLua(const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter);

    /**
     * Assign a component of type "component" loaded with "luaSelector" to "entity".
     * \param entity the entity to assign the component to
     * \param component the name of the component to add
     * \param luaSelector the lua table to load the component properties from
     */
    static void assignComponent(entityx::Entity entity, const std::string &component, const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter);

    static void registerComponent(lua::LuaState& state);

private:

};

std::ostream& operator<<(std::ostream& stream, const Component& component);

}

#endif
