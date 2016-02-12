#ifndef YAPG_GAME_SETTINGS_KEYSETTINGS_H
#define YAPG_GAME_SETTINGS_KEYSETTINGS_H

#include <map>

#include <SFML/Window/Keyboard.hpp>

namespace tinyxml2{ class XMLElement; }

namespace settings
{

class KeySettings
{
public:

    struct PlayerKeys
    {
        sf::Keyboard::Key leftKey = sf::Keyboard::Unknown;
        sf::Keyboard::Key rightKey = sf::Keyboard::Unknown;
        sf::Keyboard::Key jumpKey = sf::Keyboard::Unknown;
    };

    KeySettings() = default;

    void loadFromXml(const tinyxml2::XMLElement* elem);

    PlayerKeys& getPlayerKeys(int player);
    PlayerKeys getPlayerKeys(int player) const;

private:
    std::map<int, PlayerKeys> m_playerKeys;
};

}

#endif
