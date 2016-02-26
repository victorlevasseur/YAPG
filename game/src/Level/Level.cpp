#include "Level/Level.hpp"

#include <exception>

#include "Components/Component.hpp"
#include "Components/PlayerComponent.hpp"

namespace level
{

Level::Level(const std::string& path, lua::LuaState& luaState, LevelMode levelMode) :
    m_luaState(luaState),
    m_eventMgr(),
    m_entityMgr(m_eventMgr),
    m_levelMode(levelMode)
{
    std::cout << "Loading level \"" << path << "\"..." << std::endl;

    m_luaState.getState().open_file(path);

    //First, load all the entity and register them to the SerializedEntityGetter
    SerializedEntityGetter entityGetter;
    std::vector<entityx::Entity> createdEntities;
    m_luaState.getState()
        .get<sol::table>("level")
        .get<sol::table>("objects")
        .for_each([&](const sol::object& key, const sol::object& value) {
            //Create the entity
            entityx::Entity newEntity = m_entityMgr.create();
            createdEntities.push_back(newEntity);

            //Register it with its id
            entityGetter.registerEntity(newEntity, value.as<sol::table>().get<int>("id"));
        });

    //Now that all entities are created and registered, iterate all of them to
    //assign the components according to their template !
    unsigned int i = 1;
    for(auto it = createdEntities.begin(); it != createdEntities.end(); ++it)
    {
        std::string entityTemplateName =
            m_luaState.getState()
                .get<sol::table>("level")
                .get<sol::table>("objects")
                .get<sol::table>(i)
                .get<std::string>("template");

        const sol::table& entityParameters =
            m_luaState.getState()
                .get<sol::table>("level")
                .get<sol::table>("objects")
                .get<sol::table>(i)
                .get<sol::table>("values");

        m_luaState.getTemplate(entityTemplateName).initializeEntity(*it, entityGetter, entityParameters, /*templateComponent*/ m_levelMode == LevelMode::EditMode);

        ++i;
    }

    //Put the current level instance into "current_level" lua global variable
    luaState.getState().set("current_level", this);

    std::cout << "Level successfully loaded (" << createdEntities.size() << " entities)." << std::endl;

    //TODO: Support multiple players creation
    if(m_levelMode == LevelMode::PlayMode)
    {
        std::cout << "Creating players..." << std::endl;

        std::vector<std::string> playersTemplates{ m_luaState.getState().get<sol::table>("level").get<sol::table>("players").get<std::string>(1) };
        entityx::Entity playerEntity = m_entityMgr.create();
        m_luaState.getTemplate(playersTemplates[0]).initializeEntity(
            playerEntity,
            entityGetter,
            m_luaState.getState().get<sol::table>("level").get<sol::table>("spawn_position")
            //Directly use the "spawn_position" table as parameter ==> it implies that players
            //templates must only have x and y positions as parameters
        );
        if(!playerEntity.has_component<components::PlayerComponent>())
            throw std::runtime_error(std::string("[Lua/Error] Player entities must have the \"Player\" component declared in their template ! Not the case with \"") + playersTemplates[0] + std::string("\""));
        playerEntity.component<components::PlayerComponent>()->playerNumber = 0;

        std::cout << "Players created." << std::endl;
    }
    /////////////////////////////////////////
}

lua::EntityHandle Level::createNewEntity(const std::string& templateName)
{
    entityx::Entity newEntity = m_entityMgr.create();

    m_luaState.getTemplate(templateName).initializeEntity(newEntity, SerializedEntityGetter());

    return lua::EntityHandle(newEntity);
}

void Level::registerClass(lua::LuaState& luaState)
{
    sol::constructors<> ctor;
    sol::usertype<Level> levelLuaClass(ctor,
        "create_new_entity", &Level::createNewEntity
    );
    luaState.getState().set_usertype("level", levelLuaClass);
}

}
