#include "Settings/KeySettings.hpp"

#include "Settings/tinyxml2.h"
#include "Tools/KeyStrings.hpp"

namespace tx2 = tinyxml2;

namespace yapg
{

KeySettings::KeySettings() :
    m_playerKeys{
        {
            0,
            PlayerKeys{ sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W }
        }
        //TODO: Add other players defaults there
    }
{

}

namespace
{
    void queryKeyAttribute(const tx2::XMLElement* element, const char* attribute, sf::Keyboard::Key* key)
    {
        if(element->Attribute(attribute))
        {
            std::string keyName(element->Attribute(attribute));
            *key = stringToKey(keyName);
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

            m_playerKeys[playerId] = playerKeys;
        }
    }
}

void KeySettings::saveToXml(tx2::XMLDocument* doc, tx2::XMLElement* elem) const
{
    for(auto it = m_playerKeys.cbegin(); it != m_playerKeys.cend(); ++it)
    {
        tx2::XMLElement* playerElem = doc->NewElement("player");
        playerElem->SetAttribute("id", it->first);
        playerElem->SetAttribute("left_key", keyToString(it->second.leftKey).data());
        playerElem->SetAttribute("right_key", keyToString(it->second.rightKey).data());
        playerElem->SetAttribute("jump_key", keyToString(it->second.jumpKey).data());

        elem->InsertEndChild(playerElem);
    }
}

KeySettings::PlayerKeys& KeySettings::getPlayerKeys(int player)
{
    return m_playerKeys[player];
}

KeySettings::PlayerKeys KeySettings::getPlayerKeys(int player) const
{
    if(m_playerKeys.count(player))
        return m_playerKeys.at(player);
    else
        return PlayerKeys();
}

}
