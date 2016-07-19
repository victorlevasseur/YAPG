#ifndef YAPG_GAME_LUA_LUASTATE_H
#define YAPG_GAME_LUA_LUASTATE_H

#include <map>
#include <string>

#include "Lua/Template/EntityTemplate.hpp"
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

    const std::map<std::string, TemplatesPackage>& getPackages() const { return m_templatesPackages; }
    std::map<std::string, TemplatesPackage>& getPackages() { return m_templatesPackages; }

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
    void loadPackages(const std::string& path);
    void loadPackages(const boost::filesystem::path& path);

    sol::state m_luaState;
    std::map<std::string, TemplatesPackage> m_templatesPackages;
};

}

#include "Lua/LuaState.inl"

#endif
