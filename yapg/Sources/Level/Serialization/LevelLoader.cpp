#include "Level/Serialization/LevelLoader.hpp"

#include <iostream>

namespace yapg
{

namespace
{
    std::pair<std::string, std::string> decomposeTemplateName(const std::string& name)
    {
        if(name.size() <= 1 && name.rfind(".") < name.size() - 1)
            return std::make_pair(std::string(), std::string());

        return std::make_pair(
            name.substr(0, name.rfind(".") - 1),
            name.substr(name.rfind(".") + 1, name.size() - 1)
        );
    }
}

LevelLoader::LevelLoader(const std::string& filepath) :
    m_levelDocument(),
    m_spawnPosition(),
    m_playersTemplates(),
    m_entities()
{
    if(m_levelDocument.LoadFile(filepath.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        throw std::runtime_error("[Level/Error] Can't load the level \"" + filepath + "\" ! Not found or parsing error !");
    }

    tinyxml2::XMLElement* levelElement = m_levelDocument.RootElement();
    if(!levelElement)
    {
        throw std::runtime_error("[Level/Error] Can't find <level> root markup in the level !");
    }

    //Load the spawn position
    tinyxml2::XMLElement* spawnPosElement = levelElement->FirstChildElement("spawn_position");
    if(!spawnPosElement)
    {
        throw std::runtime_error("[Level/Error] Can't find <spawn_position> markup in the level !");
    }
    if(spawnPosElement->QueryFloatAttribute("x", &(m_spawnPosition.x)) != tinyxml2::XML_NO_ERROR || spawnPosElement->QueryFloatAttribute("y", &(m_spawnPosition.y)) != tinyxml2::XML_NO_ERROR)
    {
        throw std::runtime_error("[Level/Error] Can't find x and y in <spawn_position> markup in the level !");
    }

    //Load the players templates names
    tinyxml2::XMLElement* playersElement = levelElement->FirstChildElement("players");
    if(!playersElement)
    {
        throw std::runtime_error("[Level/Error] Can't find <players> markup in the level !");
    }
    for(tinyxml2::XMLElement* playerElem = playersElement->FirstChildElement("player");
        playerElem != nullptr;
        playerElem = playersElement->NextSiblingElement("player"))
    {
        const char* playerTemplate = playerElem->Attribute("entity_template");
        if(!playerTemplate)
        {
            std::cout << "[Level/Warning] Invalid player template !" << std::endl;
            continue;
        }
        m_playersTemplates.push_back(std::string(playerTemplate));
    }

    //First, load all the entity and register them to the SerializedEntityGetter
    tinyxml2::XMLElement* objectsElement = levelElement->FirstChildElement("objects");
    if(!objectsElement)
    {
        throw std::runtime_error("[Level/Error] Can't find <object> markup in the level !");
    }

    for(tinyxml2::XMLElement* objectElem = objectsElement->FirstChildElement("object");
        objectElem != nullptr;
        objectElem = objectElem->NextSiblingElement("object"))
    {
        SerializedEntity loadedEntity;

        if(objectElem->QueryIntAttribute("id", &(loadedEntity.id)) != tinyxml2::XML_NO_ERROR)
        {
            std::cout << "[Level/Warning] Entity without id found, it has been ignored !" << std::endl;
            continue;
        }

        if(!objectElem->Attribute("template"))
        {
            std::cout << "[Level/Warning] Entity without template specified, it has been ignored !" << std::endl;
            continue;
        }
        loadedEntity.templateName = std::string(objectElem->Attribute("template"));

        loadedEntity.parametersElement = objectElem->FirstChildElement("parameters");
        if(!loadedEntity.parametersElement)
        {
            std::cout << "[Level/Warning] Entity without parameters, it has been ignored !" << std::endl;
            continue;
        }

        m_dependencies.insert(decomposeTemplateName(loadedEntity.templateName).first);
        m_entities.push_back(std::move(loadedEntity));
    }
}

}
