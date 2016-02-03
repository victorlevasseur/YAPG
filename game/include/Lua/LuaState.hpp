#ifndef YAPG_GAME_LUA_LUASTATE_H
#define YAPG_GAME_LUA_LUASTATE_H

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

private:
    void loadTemplates(const std::string& path);

    void loadTemplates(const boost::filesystem::path& path);

    sol::state m_luaState;
};

}

#endif
