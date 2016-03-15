#include "Components/LuaDataComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

LuaDataComponent::LuaDataComponent(lua_State* L) :
    Component(),
    m_luaDataTable()
{
    //Create an empty lua table
    lua_createtable(L, 0, 0);
    m_luaDataTable = sol::table(L);
    lua_pop(L, 1);
}

LuaDataComponent::~LuaDataComponent()
{

}

std::string LuaDataComponent::getName() const
{
    return "LuaData";
}

void LuaDataComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<LuaDataComponent>()
        .setExtraLoadFunction([](LuaDataComponent* luaData, const sol::object& luaObject)
        {
            sol::object luaTable = luaObject.as<sol::table>().get<sol::object>("data");
            if(luaTable.is<sol::table>())
                luaData->m_luaDataTable = luaTable;
            else
            {
                //Create an empty lua table
                lua_createtable(luaObject.state(), 0, 0);
                luaData->m_luaDataTable = sol::table(luaObject.state());
                lua_pop(luaObject.state(), 1);
            }
        });

    lua::EntityHandle::declareComponent<LuaDataComponent>("LuaData");
}

std::ostream& operator<<(std::ostream& stream, const LuaDataComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
