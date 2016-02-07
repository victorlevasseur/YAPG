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

    int getTableSize(const std::string& tableName);
    std::vector<std::string> getTableKeys(const std::string& tableName);

    const EntityTemplate& getTemplate(const std::string& name) const;

private:
    void loadTemplates(const std::string& path);

    void loadTemplates(const boost::filesystem::path& path);

    sol::state m_luaState;
    std::map<std::string, EntityTemplate> m_templates;
};

}

#endif
