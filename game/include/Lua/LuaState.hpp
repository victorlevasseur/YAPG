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
    
private:
    void loadTemplates(const std::string& path);
    
    void loadTemplates(const boost::filesystem::path& path);

    sel::State m_luaState;
};

}
}

#endif