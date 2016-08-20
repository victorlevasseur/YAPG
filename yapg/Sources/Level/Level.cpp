#include "Level/Level.hpp"

#include <exception>

#include "Common/Component.hpp"
#include "Level/Serialization/LevelLoader.hpp"
#include "Template/TemplateComponent.hpp"
#include "Player/PlayerComponent.hpp"
#include "Settings/tinyxml2.h"

namespace yapg
{

Level::Level(LuaState& luaState, const LevelLoader& loader) :
    m_luaState(luaState),
    m_eventMgr(),
    m_entityMgr(m_eventMgr),
    m_spawnPosition(loader.getSpawnPosition()),
    m_playersTemplates(loader.getPlayersTemplatesBegin(), loader.getPlayersTemplatesEnd()),
    m_nextId(0)
{
    SerializedEntityGetter entityGetter;
    std::vector<entityx::Entity> createdEntities;

    //Create all entities
    for(auto it = loader.getEntitiesBegin(); it != loader.getEntitiesEnd(); ++it)
    {
        //Create the entity
        entityx::Entity newEntity = m_entityMgr.create();
        createdEntities.push_back(newEntity);

        //Register the entity with its ID
        entityGetter.registerEntity(newEntity, it->id);
        m_nextId = std::max(m_nextId, it->id);
    }

    //Apply the templates to all the entities
    unsigned int i = 0;
    for(auto it = loader.getEntitiesBegin(); it != loader.getEntitiesEnd(); ++it)
    {
        m_luaState.getTemplate(it->templateName).initializeEntity(createdEntities[i], entityGetter, it->parametersElement);
        ++i;
    }

    std::cout << "Level successfully loaded (" << createdEntities.size() << " entities)." << std::endl;

    //Note: the player's creation is done in LevelState.
}

void Level::saveToFile(const std::string& path)
{
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

std::vector<entityx::Entity> Level::getEntities(const std::string& templateName)
{
    std::vector<entityx::Entity> entities;
    m_entityMgr.each<TemplateComponent>([&](entityx::Entity entity, TemplateComponent& templateComponent)
    {
        if(templateComponent.templateName == templateName)
        {
            entities.push_back(entity);
        }
    });

    return entities;
}

void Level::registerClass(LuaState& luaState)
{

}

}
