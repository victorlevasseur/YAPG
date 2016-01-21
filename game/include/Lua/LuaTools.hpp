#ifndef YAPG_GAME_LUA_LUATOOLS_H
#define YAPG_GAME_LUA_LUATOOLS_H

#include <functional>

#include "selene.h"

namespace game
{
namespace lua
{

void iterateOnArrayTable(sel::Selector table, const std::function<void(unsigned int, sel::Selector)>& function);

}
}

#endif
