#include "Lua/EntityTemplate.hpp"

#include "Components/Component.hpp"
#include "Lua/EntityHandle.hpp"

namespace lua
{

EntityTemplate::EntityTemplate(const sol::table& templateTable) :
    m_name(),
    m_friendlyName(),
    m_parameters(),
    m_componentsTable()
{
    m_name = templateTable.get<std::string>("name");
    m_friendlyName = templateTable.get<std::string>("friendlyname");

    m_componentsTable = templateTable.get<sol::table>("components");

    //Load the parameters
    templateTable.get<sol::table>("parameters").for_each([&](const sol::object &key, const sol::object &value)
    {
        sol::table valueTable = value.as<sol::table>();
        std::string type = valueTable.get<std::string>("type");

        m_parameters.emplace(
            key.as<std::string>(),
            Parameter{
                key.as<std::string>(),
                valueTable.get<std::string>("name"),
                valueTable.get<std::string>("component"),
                valueTable.get<std::string>("attribute"),
                (type == "string" ? Parameter::String :
                    (type == "number" ? Parameter::Number :
                    (type == "boolean" ? Parameter::Boolean :
                    (type == "entity" ? Parameter::Entity :
                    (type == "function" ? Parameter::Function :
                    Parameter::Unknown
                )))))
            }
        );
    });
}

void EntityTemplate::initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter) const
{
    //Add each components to the entity
    m_componentsTable.for_each([&](const sol::object& key, const sol::object& value) {
        std::string componentType = key.as<std::string>();

        components::Component::assignComponent(entity, componentType, value, entityGetter);
    });
}

void EntityTemplate::initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, const sol::table& parametersTable) const
{
    initializeEntity(entity, entityGetter);
    for(auto it = m_parameters.cbegin(); it != m_parameters.cend(); ++it)
    {
        sol::object parameterValue = parametersTable.get<sol::object>(it->first);
        if(it->second.attributeType == Parameter::String && parameterValue.is<std::string>())
        {
            lua::EntityHandle(entity).setAttributeAsString(it->second.component, it->second.attribute, parameterValue.as<std::string>());
        }
        else if(it->second.attributeType == Parameter::Number && parameterValue.is<double>())
        {
            lua::EntityHandle(entity).setAttributeAsDouble(it->second.component, it->second.attribute, parameterValue.as<double>());
        }
        else if(it->second.attributeType == Parameter::Boolean && parameterValue.is<bool>())
        {
            lua::EntityHandle(entity).setAttributeAsBool(it->second.component, it->second.attribute, parameterValue.as<bool>());
        }
        //TODO: Do it for other supported type in parameters (Entity and Function)
        else
        {
            std::cout << "[Lua/Warning] Template \"" << m_name << "\" needs the parameter \"" << it->first << "\" but not given by the instanciated entity or not of the correct type !" << std::endl;
        }
    }
}

}
