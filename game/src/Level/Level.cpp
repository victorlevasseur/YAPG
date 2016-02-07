#include "Level/Level.hpp"

#include "Components/Component.hpp"

namespace level
{

Level::Level(const std::string& path, lua::LuaState& luaState) :
    m_luaState(luaState),
    m_eventMgr(),
    m_entityMgr(m_eventMgr)
{
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

        //Get the components of the template
        sol::table entityComponentsTable =
            m_luaState.getState()
                .get<sol::table>(entityTemplateName)
                .get<sol::table>("components");

        //Add each components
        entityComponentsTable.for_each([&](const sol::object& key, const sol::object& value) {
            std::string componentType = key.as<std::string>();

            components::Component::assignComponent(*it, componentType, value, entityGetter);
        });

        //Then, affect the X and Y position
        //(TODO: Support all types of parameters declared in the template's "parameters" section)
        sol::table valuesForParameters =
            m_luaState.getState()
                .get<sol::table>("level")
                .get<sol::table>("objects")
                .get<sol::table>(i)
                .get<sol::table>("values");
        lua::EntityHandle(*it).setAttributeAsDouble("Position", "x", valuesForParameters.get<double>("x"));
        lua::EntityHandle(*it).setAttributeAsDouble("Position", "y", valuesForParameters.get<double>("y"));

        ++i;
    }

    //Put the current level instance into "current_level" lua global variable
    luaState.getState().set("current_level", this);
}

lua::EntityHandle Level::createNewEntity(const std::string& templateName)
{
    if(m_luaState.getState().get<sol::object>(templateName).is<sol::table>())
    {
        entityx::Entity newEntity = m_entityMgr.create();

        sol::table templateTable = m_luaState.getState().get<sol::table>(templateName);
        templateTable.get<sol::table>("components").for_each([&](const sol::object& key, const sol::object& value) {
            std::string componentType = key.as<std::string>();

            components::Component::assignComponent(newEntity, componentType, value, SerializedEntityGetter());
        });

        return lua::EntityHandle(newEntity);
    }

    std::cout << "[Lua/Warning] Can't create an entity from template \"" << templateName << "\"" << std::endl;
    return lua::EntityHandle();
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
