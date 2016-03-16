#include "Components/LuaDataComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

LuaDataComponent::LuaDataComponent() :
    Component(),
    m_dataTable()
{

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
            //TODO
        });

    lua::EntityHandle::declareComponent<LuaDataComponent>("LuaData");

    state.getState().new_usertype<LuaDataComponent>("data_component",
        "has_value", &LuaDataComponent::hasValue,
        "get_value", &LuaDataComponent::getValue,
        "set_value", &LuaDataComponent::setValue
    );
}

bool LuaDataComponent::hasValue(const std::string& key) const
{
    return m_dataTable.count(key) > 0;
}

boost::any LuaDataComponent::getValue(const std::string& key) const
{
    return m_dataTable.at(key);
    //TODO: Return an empty boost::any if doesn't exists
}

void LuaDataComponent::setValue(const std::string& key, const boost::any& value)
{
    m_dataTable[key] = value;
}

std::ostream& operator<<(std::ostream& stream, const LuaDataComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
