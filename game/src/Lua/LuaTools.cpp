#include "Lua/LuaTools.hpp"

namespace game
{
namespace lua
{

void iterateOnArrayTable(sel::Selector table, const std::function<void(unsigned int, sel::Selector)>& function)
{
    unsigned int i = 1;
    while((void*)table[i] != nullptr)
    {
        function(i, table[i]);
        ++i;
    }
}

}
}
