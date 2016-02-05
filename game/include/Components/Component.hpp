#ifndef YAPG_GAME_COMPONENTS_COMPONENT_H
#define YAPG_GAME_COMPONENTS_COMPONENT_H

#include <map>
#include <string>

#include "entityx/entityx.h"

#include "Level/SerializedEntityGetter.hpp"
#include "Lua/sol.hpp"
#include "Meta/Metadata.hpp"

/**
 * Macro that has to be added to the end of component class declaration.
 * It declares some useful methods to get/set attributes from Lua.
 */
#define DECLARE_COMPONENT(T) \
    protected: \
    virtual std::string doGetAttributeAsString(const std::string& attributeName) const \
    {\
        auto& metadata = dynamic_cast<meta::ClassMetadata<T>&>(\
            meta::MetadataStore::getMetadata<T>()\
        );\
        return metadata.getAttribute(attributeName).getAsString(this);\
    }\

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

    std::string getAttributeAsString(const std::string& attributeName) const;

protected:
    virtual std::string doGetAttributeAsString(const std::string& attributeName) const
    {
        std::cout << "Invalid component ! Did you put the DECLARE_COMPONENT(MyComponent) before the end of the component class declaration ?" << std::endl;
        return "";
    }

private:

};

std::ostream& operator<<(std::ostream& stream, const Component& component);

}

#endif
