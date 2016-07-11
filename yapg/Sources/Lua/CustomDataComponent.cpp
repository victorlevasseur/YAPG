#include "Lua/CustomDataComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

CustomDataComponent::CustomDataComponent(entityx::Entity entity) :
    Component(entity),
    m_dataTable()
{

}

CustomDataComponent::~CustomDataComponent()
{

}

std::string CustomDataComponent::getName() const
{
    return "CustomData";
}

void CustomDataComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<CustomDataComponent>()
        .setExtraLoadFunction([](CustomDataComponent* CustomData, const sol::object& luaObject)
        {
            if(luaObject.is<sol::table>())
            {
                sol::table luaTable = luaObject.as<sol::table>();
                luaTable.for_each([&](const sol::object& key, const sol::object& value)
                {
                    if(!key.is<std::string>())
                    {
                        std::cout << "[Lua/Error] CustomDataComponent table keys must be strings!" << std::endl;
                        return;
                    }
                    if(!value.is<boost::any>())
                    {
                        std::cout << "[Lua/Error] CustomDataComponent table value must be boost::any (constructed with XXX_value(...) lua function)!" << std::endl;
                        return;
                    }

                    CustomData->setValue(key.as<std::string>(), value.as<boost::any>());
                });
            }
            else
            {
                std::cout << "[Lua/Error] Can't load CustomDataComponent: not a table!" << std::endl;
            }
        });

    lua::EntityHandle::declareComponent<CustomDataComponent>("custom_data");

    state.getState().new_usertype<CustomDataComponent>("data_component",
        "has_value", &CustomDataComponent::hasValue,
        "get_value", &CustomDataComponent::getValue,
        "set_value", &CustomDataComponent::setValue
    );
}

bool CustomDataComponent::hasValue(const std::string& key) const
{
    return m_dataTable.count(key) > 0;
}

boost::any CustomDataComponent::getValue(const std::string& key) const
{
    return m_dataTable.at(key);
    //TODO: Return an empty boost::any if doesn't exists
}

void CustomDataComponent::setValue(const std::string& key, const boost::any& value)
{
    m_dataTable[key] = value;
}

std::ostream& operator<<(std::ostream& stream, const CustomDataComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
