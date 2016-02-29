#ifndef YAPG_GAME_LUA_LUASTATE_H
#define YAPG_GAME_LUA_LUASTATE_H

#include <map>
#include <string>

#include "Lua/EntityTemplate.hpp"
#include "Lua/sol.hpp"

namespace boost{ namespace filesystem{ class path; } }

namespace lua
{

class LuaState
{
public:
    LuaState();

    sol::state& getState();
    const sol::state& getState() const;

    const EntityTemplate& getTemplate(const std::string& name) const;

    const std::map<std::string, EntityTemplate>& getTemplates() const { return m_templates; }
    std::map<std::string, EntityTemplate>& getTemplates() { return m_templates; }

    template<class T>
    void declareAnyConvertibleType(const std::string& typeName);

    /**
     * Merge the two tables into a single one.
     * None of the two table are overriden.
     */
    sol::table mergeTables(sol::table first, sol::table second);

private:
    void loadTemplates(const std::string& path);

    void loadTemplates(const boost::filesystem::path& path);

    sol::state m_luaState;
    std::map<std::string, EntityTemplate> m_templates;
};

}

#include "Lua/LuaState.inl"

#endif
