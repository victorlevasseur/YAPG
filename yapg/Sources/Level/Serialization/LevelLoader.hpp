#ifndef YAPG_GAME_LEVEL_SERIALIZATION_LEVELLOADER_H
#define YAPG_GAME_LEVEL_SERIALIZATION_LEVELLOADER_H

#include <set>
#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Settings/tinyxml2.h"

namespace yapg
{

class LevelLoader
{
public:
    struct SerializedEntity
    {
        int id = -1;
        std::string templateName;
        tinyxml2::XMLElement* parametersElement;
    };

    LevelLoader(const std::string& filepath);

    sf::Vector2f getSpawnPosition() const { return m_spawnPosition; }

    std::vector<std::string>::const_iterator getPlayersTemplatesBegin() const { return m_playersTemplates.cbegin(); }
    std::vector<std::string>::const_iterator getPlayersTemplatesEnd() const { return m_playersTemplates.cend(); }

    std::vector<SerializedEntity>::const_iterator getEntitiesBegin() const { return m_entities.cbegin(); }
    std::vector<SerializedEntity>::const_iterator getEntitiesEnd() const { return m_entities.cend(); }

    std::set<std::string>::const_iterator getDependenciesBegin() const { return m_dependencies.cbegin(); }
    std::set<std::string>::const_iterator getDependenciesEnd() const { return m_dependencies.cend(); }

private:
    tinyxml2::XMLDocument m_levelDocument;

    sf::Vector2f m_spawnPosition;
    std::vector<std::string> m_playersTemplates;
    std::vector<SerializedEntity> m_entities;

    std::set<std::string> m_dependencies;
};

}

#endif
