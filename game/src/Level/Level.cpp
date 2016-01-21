#include "Level/Level.hpp"

#include "Components/Component.hpp"
#include "Lua/EntityHandle.hpp"

namespace game
{
namespace level
{

Level::Level(const std::string& path, lua::LuaState& luaState) :
    m_luaState(luaState),
    m_eventMgr(),
    m_entityMgr(m_eventMgr)
{
    m_luaState.getState().open_file(path);

    //Calculate the level objects table length
    unsigned int objectsCount = m_luaState.getTableSize("level.objects");

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
    //assign the components
    unsigned int i = 1;
    for(auto it = createdEntities.begin(); it != createdEntities.end(); ++it)
    {
        sol::table entityComponentsTable =
            m_luaState.getState()
                .get<sol::table>("level")
                .get<sol::table>("objects")
                .get<sol::table>(i)
                .get<sol::table>("components");

        //Add each components
        entityComponentsTable.for_each([&](const sol::object& key, const sol::object& value) {
            std::string componentType = key.as<std::string>();
            sol::table componentTable = value.as<sol::table>();

            components::Component::assignComponent(*it, componentType, componentTable, entityGetter);
        });

        ++i;
    }
}

}
}
