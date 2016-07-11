#ifndef YAPG_GAME_TOOLS_STRINGTOOLS_H
#define YAPG_GAME_TOOLS_STRINGTOOLS_H

#include <string>

#include <SFML/System/String.hpp>

namespace tools
{

std::u32string getUTF32String(const std::string& str)
{
    sf::String tempStr(str);
    return std::u32string(reinterpret_cast<const char32_t*>(tempStr.getData()));
}

std::string getLocalString(const std::u32string& str32)
{
    sf::String tempStr(reinterpret_cast<const uint32_t*>(str32.data()));
    return tempStr.toAnsiString();
}

}

#endif
