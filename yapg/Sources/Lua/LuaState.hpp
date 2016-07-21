#ifndef YAPG_GAME_LUA_LUASTATE_H
#define YAPG_GAME_LUA_LUASTATE_H

#include <map>
#include <string>

#include "Template/EntityTemplate.hpp"
#include "Lua/sol.hpp"

namespace boost{ namespace filesystem{ class path; } }

namespace yapg
{

struct TemplatesPackage
{
    std::string packageName;
    std::string packageFriendlyName;
    std::map<std::string, EntityTemplate> templates;
};

class LuaState
{
public:
    LuaState(bool loadAllTemplates = true);

    sol::state& getState();
    const sol::state& getState() const;

    const EntityTemplate& getTemplate(const std::string& name) const;
    bool hasTemplate(const std::string& name) const;

    void loadAllTemplates();
    void unloadAllTemplates();
    
    EntityTemplate& declareTemplate(EntityTemplate&& entityTemplate);

    const std::map<std::string, EntityTemplate>& getTemplates() const { return m_templates; }
    std::map<std::string, EntityTemplate>& getTemplates() { return m_templates; }

    template<class T>
    void declareAnyConvertibleType(const std::string& typeName);

    template<class T>
    void declareComponentGetter(const std::string& componentLuaName);

    /**
     * Merge the two tables into a single one.
     * None of the two table are overriden.
     */
    sol::table mergeTables(sol::table first, sol::table second);

private:

    sol::state m_luaState;
    std::map<std::string, EntityTemplate> m_templates;
};

}

#include "Lua/LuaState.inl"

#endif
