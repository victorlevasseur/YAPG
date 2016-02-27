#include "Lua/EntityTemplate.hpp"

#include "Components/Component.hpp"
#include "Components/TemplateComponent.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/LuaState.hpp"
#include "Settings/tinyxml2.h"

namespace lua
{

EntityTemplate::EntityTemplate(const sol::table& templateTable) :
    m_name(),
    m_friendlyName(),
    m_inheritedTemplate(),
    m_parameters(),
    m_componentsTable()
{
    m_name = templateTable.get<std::string>("name");
    m_friendlyName = templateTable.get<std::string>("friendlyname");

    if(templateTable.get<sol::object>("inherits").is<std::string>())
        m_inheritedTemplate = templateTable.get<std::string>("inherits");

    m_componentsTable = templateTable.get<sol::table>("components");

    //Load the parameters
    if(templateTable.get<sol::object>("parameters").is<sol::table>())
    {
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
}

void EntityTemplate::applyInheritance(LuaState& luaState)
{
    if(!m_inheritedTemplate.empty())
    {
        //Merge the parameters
        m_parameters.insert(
            luaState.getTemplate(m_inheritedTemplate).m_parameters.begin(),
            luaState.getTemplate(m_inheritedTemplate).m_parameters.end()
        );

        //Merge the component lua table
        m_componentsTable = luaState.mergeTables(luaState.getTemplate(m_inheritedTemplate).m_componentsTable, m_componentsTable);
    }
}

void EntityTemplate::initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, bool templateComponent) const
{
    //Add each components to the entity
    m_componentsTable.for_each([&](const sol::object& key, const sol::object& value) {
        std::string componentType = key.as<std::string>();

        components::Component::assignComponent(entity, componentType, value, entityGetter);
    });

    if(templateComponent)
    {
        //Add the template component, containing infos about the template
        entity.assign<components::TemplateComponent>(entityGetter.getSerializationIdFromEntity(entity), getName());
    }
}

void EntityTemplate::initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, const tinyxml2::XMLElement* parametersElement, bool templateComponent) const
{
    initializeEntity(entity, entityGetter);
    for(auto it = m_parameters.cbegin(); it != m_parameters.cend(); ++it)
    {
        try
        {
            const tinyxml2::XMLElement* parameterElement = parametersElement->FirstChildElement(it->first.c_str());
            if(!parameterElement)
            {
                throw std::runtime_error("Template \"" + m_name + "\" needs the parameter \"" + it->first + "\" but not given by the instanciated object !");
            }

            //String parameter
            if(it->second.attributeType == Parameter::String)
            {
                const char* stringValue = parameterElement->GetText();
                if(!stringValue)
                {
                    throw std::runtime_error("Template \"" + m_name + "\" needs the parameter \"" + it->first + "\" but not of the correct type !");
                }

                lua::EntityHandle(entity).setAttributeAsString(it->second.component, it->second.attribute, std::string(stringValue));
            }
            else if(it->second.attributeType == Parameter::Number)
            {
                double doubleValue = 0.0;
                if(parameterElement->QueryDoubleText(&doubleValue) != tinyxml2::XML_SUCCESS)
                {
                    throw std::runtime_error("Template \"" + m_name + "\" needs the parameter \"" + it->first + "\" but not of the correct type !");
                }

                lua::EntityHandle(entity).setAttributeAsDouble(it->second.component, it->second.attribute, doubleValue);
            }
            else if(it->second.attributeType == Parameter::Boolean)
            {
                bool boolValue = false;
                if(!parameterElement->GetText())
                {
                    throw std::runtime_error("Template \"" + m_name + "\" needs the parameter \"" + it->first + "\" but not of the correct type !");
                }
                boolValue = (strcmp(parameterElement->GetText(), "True") == 0);

                lua::EntityHandle(entity).setAttributeAsBool(it->second.component, it->second.attribute, boolValue);
            }

        }
        catch(std::exception& e)
        {
            std::cout << "[Lua/Warning] " << e.what() << std::endl;
        }
    }
}

}
