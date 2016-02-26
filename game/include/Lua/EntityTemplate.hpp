#ifndef YAPG_GAME_LUA_ENTITYTEMPLATE_H
#define YAPG_GAME_LUA_ENTITYTEMPLATE_H

#include "entityx/entityx.h"

#include <map>
#include <string>

#include "Level/SerializedEntityGetter.hpp"
#include "Lua/sol.hpp"

namespace lua{ class LuaState; }

namespace lua
{

class EntityTemplate
{
public:

    struct Parameter
    {
        std::string name;
        std::string friendlyName;

        std::string component;
        std::string attribute;
        enum
        {
            String,
            Number,
            Boolean,
            Entity,
            Function,
            Unknown
        } attributeType;
    };

    EntityTemplate(const sol::table& templateTable);
    ~EntityTemplate() = default;

    EntityTemplate(const EntityTemplate&) = delete;
    EntityTemplate& operator=(const EntityTemplate&) = delete;

    EntityTemplate(EntityTemplate&&) = default;
    EntityTemplate& operator=(EntityTemplate&&) = default;

    void applyInheritance(LuaState& luaState);

    void initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, bool templateComponent = false) const;

    void initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, const sol::table& parametersTable, bool templateComponent = false) const;

    std::string getName() const { return m_name; }

    std::string getFriendlyName() const { return m_friendlyName; }

    const std::map<std::string, Parameter>& getParameters() const { return m_parameters; }

private:
    std::string m_name;
    std::string m_friendlyName;

    std::string m_inheritedTemplate;

    std::map<std::string, Parameter> m_parameters; ///< The parameters of the template

    sol::table m_componentsTable; ///< The "components" table
};

}

#endif
