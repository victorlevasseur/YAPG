#include "Settings/KeySettings.hpp"

#include "Settings/tinyxml2.h"
#include "Tools/KeyStrings.hpp"

namespace tx2 = tinyxml2;

namespace settings
{

namespace
{
    void queryKeyAttribute(const tx2::XMLElement* element, const char* attribute, sf::Keyboard::Key* key)
    {
        if(element->Attribute(attribute))
        {
            std::string keyName(element->Attribute(attribute));
            *key = tools::stringToKey(keyName);
        }
    }
}

void KeySettings::loadFromXml(const tx2::XMLElement* elem)
{
    for(const tx2::XMLElement* playerElem = elem->FirstChildElement("player");
        playerElem != nullptr;
        playerElem = playerElem->NextSiblingElement("player"))
    {
        int playerId = -1;
        playerElem->QueryIntAttribute("id", &playerId);
        if(playerId >= 0)
        {
            PlayerKeys playerKeys;
            queryKeyAttribute(playerElem, "left_key", &playerKeys.leftKey);
            queryKeyAttribute(playerElem, "right_key", &playerKeys.rightKey);
            queryKeyAttribute(playerElem, "jump_key", &playerKeys.jumpKey);

            m_playerKeys.emplace(playerId, playerKeys);
        }
    }
}

KeySettings::PlayerKeys& KeySettings::getPlayerKeys(int player)
{
    return m_playerKeys[player];
}

const KeySettings::PlayerKeys& KeySettings::getPlayerKeys(int player) const
{
    return m_playerKeys.at(player);
}

}
