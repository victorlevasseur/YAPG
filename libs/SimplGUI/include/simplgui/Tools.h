#ifndef SIMPLGUI_TOOLS_H
#define SIMPLGUI_TOOLS_H

namespace simplgui
{

namespace tools
{

static sf::String getSfString(const std::u32string &str)
{
    if(str.empty())
        return sf::String();
    else
        return sf::String(reinterpret_cast<const sf::Uint32*>(str.c_str()));
}

}

}

#endif
