#ifndef YAPG_GAME_LUA_LUASTATE_H
#define YAPG_GAME_LUA_LUASTATE_H

#include "selene.h"

namespace boost{ namespace filesystem{ class path; } }

namespace game
{
namespace lua
{

class LuaState
{
public:
    LuaState();

    sel::State& getState();
    const sel::State& getState() const;

    int getTableSize(const std::string& tableName);
    std::vector<std::string> getTableKeys(const std::string& tableName);

private:
    void loadTemplates(const std::string& path);

    void loadTemplates(const boost::filesystem::path& path);

    sel::State m_luaState;
};

}
}

#endif
