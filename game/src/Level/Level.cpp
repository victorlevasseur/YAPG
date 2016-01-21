#include "Level/Level.hpp"

#include "Components/Component.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/LuaTools.hpp"

namespace game
{
namespace level
{

Level::Level(const std::string& path, lua::LuaState& luaState) :
    m_luaState(luaState),
    m_eventMgr(),
    m_entityMgr(m_eventMgr)
{
    m_luaState.getState().Load(path);

    //Calculate the level objects table length
    unsigned int objectsCount = m_luaState.getTableSize("level.objects");

    //First, load all the entity and register them to the SerializedEntityGetter
    SerializedEntityGetter entityGetter;
    std::vector<entityx::Entity> createdEntities;
    lua::iterateOnArrayTable(m_luaState.getState()["level"]["objects"], [&](int, sel::Selector entitySel) {
        //Create the entity
        entityx::Entity newEntity = m_entityMgr.create();
        createdEntities.push_back(newEntity);

        //Register it with its id
        entityGetter.registerEntity(newEntity, entitySel["id"]);
    });

    //Now that all entities are created and registered, iterate all of them to
    //assign the components
    unsigned int i = 1;
    for(auto it = createdEntities.begin(); it != createdEntities.end(); ++it)
    {
        sel::Selector entityComponentsSel =
            m_luaState.getState()["level"]["objects"][i]["components"];

        //Get the components
        std::vector<std::string> componentsList =
            m_luaState.getTableKeys(
                "level.objects[" + std::to_string(i) + "].components"
            );

        //Assign each components
        for(auto it2 = componentsList.cbegin(); it2 != componentsList.cend(); ++it2)
        {
            sel::Selector entityComponentSel = entityComponentsSel[*it2];
            components::Component::assignComponent(*it, *it2, entityComponentSel, entityGetter);
        }

        ++i;
    }
}

}
}
