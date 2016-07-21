#include "Level/Level.hpp"

#include <exception>

#include "Common/Component.hpp"
#include "Template/TemplateComponent.hpp"
#include "Player/PlayerComponent.hpp"
#include "Settings/tinyxml2.h"

namespace yapg
{

Level::Level(LuaState& luaState, LevelMode levelMode) :
    m_luaState(luaState),
    m_eventMgr(),
    m_entityMgr(m_eventMgr),
    m_spawnPosition(),
    m_playersTemplates(),
    m_levelMode(levelMode),
    m_nextId(0)
{

}

void Level::LoadFromFile(const std::string& path)
{
    for(entityx::Entity entity : m_entityMgr.entities_for_debugging())
        entity.destroy();
    m_playersTemplates.clear();
    m_nextId = 0;

    std::cout << "Loading level \"" << path << "\"..." << std::endl;

    tinyxml2::XMLDocument levelDocument;
    if(levelDocument.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        throw std::runtime_error("[Level/Error] Can't load the level \"" + path + "\" ! Not found or parsing error !");
    }

    tinyxml2::XMLElement* levelElement = levelDocument.RootElement();
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

    SerializedEntityGetter entityGetter;
    std::vector<entityx::Entity> createdEntities;

    for(tinyxml2::XMLElement* objectElem = objectsElement->FirstChildElement("object");
        objectElem != nullptr;
        objectElem = objectElem->NextSiblingElement("object"))
    {
        //Create the entity
        entityx::Entity newEntity = m_entityMgr.create();
        createdEntities.push_back(newEntity);

        //Register it with its id (if it has a id attribute)
        int newEntityId = -1;
        auto error = objectElem->QueryIntAttribute("id", &newEntityId);
        if(error == tinyxml2::XML_NO_ERROR)
        {
            entityGetter.registerEntity(newEntity, newEntityId);
            m_nextId = std::max(m_nextId, newEntityId);
        }
    }

    //Now that all entities are created and registered, iterate all of them to
    //assign the components according to their template !
    unsigned int i = 0;
    for(tinyxml2::XMLElement* objectElem = objectsElement->FirstChildElement("object");
        objectElem != nullptr;
        objectElem = objectElem->NextSiblingElement("object"))
    {
        if(!objectElem->Attribute("template"))
        {
            //There's no template defined, remove the entity.
            std::cout << "[Level/Warning] No template attribute defined for an <object> !" << std::endl;
            createdEntities.erase(createdEntities.begin() + i);
            continue;
        }
        std::string entityTemplateName(objectElem->Attribute("template"));

        tinyxml2::XMLElement* entityParameters = objectElem->FirstChildElement("parameters");
        if(!entityParameters)
        {
            //There's no template defined, remove the entity.
            std::cout << "[Level/Warning] No <parameters> in <object> !" << std::endl;
            createdEntities.erase(createdEntities.begin() + i);
            continue;
        }

        m_luaState.getTemplate(entityTemplateName).initializeEntity(createdEntities[i], entityGetter, entityParameters);

        ++i;
    }

    std::cout << "Level successfully loaded (" << createdEntities.size() << " entities)." << std::endl;

    //Note: the player's creation is done in LevelState.
}

void Level::SaveToFile(const std::string& path)
{
    if(m_levelMode == LevelMode::PlayMode)
    {
        throw std::runtime_error("[Level/Error] Can't save a level opened in play mode !");
    }

    tinyxml2::XMLDocument levelDocument;

    tinyxml2::XMLElement* levelElement = levelDocument.NewElement("level");
    levelDocument.LinkEndChild(levelElement);

    //Save the spawn position
    {
        tinyxml2::XMLElement* spawnElement = levelDocument.NewElement("spawn_position");
        spawnElement->SetAttribute("x", m_spawnPosition.x);
        spawnElement->SetAttribute("y", m_spawnPosition.y);

        levelElement->LinkEndChild(spawnElement);
    }

    //Save the players templates
    {
        tinyxml2::XMLElement* playersElement = levelDocument.NewElement("players");
        playersElement->SetAttribute("max_player", static_cast<int>(m_playersTemplates.size()));

        for(auto it = m_playersTemplates.cbegin(); it != m_playersTemplates.cend(); ++it)
        {
            tinyxml2::XMLElement* playerElement = levelDocument.NewElement("player");
            playerElement->SetAttribute("entity_template", (*it).c_str());

            playersElement->LinkEndChild(playerElement);
        }

        levelElement->LinkEndChild(playersElement);
    }

    //Save the entities
    {
        tinyxml2::XMLElement* objectsElement = levelDocument.NewElement("objects");

        //Go through all entities and create an XMLElement element for them and set an ID
        //Give the ID to the SerializedEntityGetter
        std::map<entityx::Entity, tinyxml2::XMLElement*> entityElements;

        SerializedEntityGetter entityGetter;
        unsigned int nextId = 1;
        m_entityMgr.each<TemplateComponent>([&entityGetter, &nextId, &entityElements, &objectsElement, &levelDocument](entityx::Entity entity, TemplateComponent& templateComponent)
        {
            entityGetter.registerEntity(entity, nextId);

            tinyxml2::XMLElement* entityElement = levelDocument.NewElement("object");
            entityElement->SetAttribute("id", static_cast<int>(nextId));

            std::string fullName = templateComponent.templateName;
            entityElement->SetAttribute("template", fullName.c_str());

            entityElements[entity] = entityElement;
            objectsElement->LinkEndChild(entityElement);

            ++nextId;
        });

        //Go through all entities again to save their parameters and entity template name
        //This allows all entities to be able to get the other's ID while saving their parameters
        m_entityMgr.each<TemplateComponent>([&](entityx::Entity entity, TemplateComponent& templateComponent)
        {
            const EntityTemplate& entityTemplate = m_luaState.getTemplate(templateComponent.templateName);

            tinyxml2::XMLElement* parametersElement = levelDocument.NewElement("parameters");
            entityElements.at(entity)->LinkEndChild(parametersElement);

            entityTemplate.saveEntity(entity, entityGetter, parametersElement);
        });

        levelElement->LinkEndChild(objectsElement);
    }

    levelDocument.SaveFile(path.c_str());
}

entityx::Entity Level::createNewEntity(const std::string& templateName)
{
    entityx::Entity newEntity = m_entityMgr.create();
    m_luaState.getTemplate(templateName).initializeEntity(newEntity, SerializedEntityGetter());

    newEntity.component<TemplateComponent>()->serializedId = m_nextId;
    ++m_nextId;

    return newEntity;
}

void Level::registerClass(LuaState& luaState)
{

}

}
