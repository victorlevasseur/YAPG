#ifndef YAPG_GAME_TOOLS_KEYSTRINGS_H
#define YAPG_GAME_TOOLS_KEYSTRINGS_H

#include <string>

#include <SFML/Window/Keyboard.hpp>

namespace tools
{

std::string keyToString(sf::Keyboard::Key key);

sf::Keyboard::Key stringToKey(const std::string& str);

}

#endif
