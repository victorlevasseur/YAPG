#include "Lua/EntityParametersHelper.hpp"

#include "Components/LuaDataComponent.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/EntityTemplate.hpp"

namespace lua
{

EntityParametersHelper::EntityParametersHelper(const EntityTemplate* entityTemplate, entityx::Entity entity) :
    m_entityTemplate(entityTemplate),
    m_entity(entity)
{

}

boost::any EntityParametersHelper::getParameter(const std::string& name) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const lua::EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == lua::EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        return lua::EntityHandle(m_entity).getAttributeAsAny(parameter.component, parameter.attribute);
    }
    else
    {
        if(lua::EntityHandle(m_entity).getLuaData()->hasValue(parameter.field))
            return lua::EntityHandle(m_entity).getLuaData()->getValue(parameter.field);
        else
            return boost::any();
    }
}

void EntityParametersHelper::setParameter(const std::string& name, const boost::any& value) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const lua::EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == lua::EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        lua::EntityHandle(m_entity).setAttributeAsAny(parameter.component, parameter.attribute, value);
    }
    else
    {
        if(lua::EntityHandle(m_entity).getLuaData()->hasValue(parameter.field))
            lua::EntityHandle(m_entity).getLuaData()->setValue(parameter.field, value);
    }
}

std::type_index EntityParametersHelper::getParameterType(const std::string& name) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const lua::EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == lua::EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        return lua::EntityHandle(m_entity).getAttributeType(parameter.component, parameter.attribute);
    }
    else
    {
        if(lua::EntityHandle(m_entity).getLuaData()->hasValue(parameter.field))
            return lua::EntityHandle(m_entity).getLuaData()->getValue(parameter.field).type();
        else
            return typeid(void);
    }
}

std::map<std::string, EntityTemplate::Parameter>::const_iterator EntityParametersHelper::parametersBegin() const
{
    return m_entityTemplate->getParameters().begin();
}

std::map<std::string, EntityTemplate::Parameter>::const_iterator EntityParametersHelper::parametersEnd() const
{
    return m_entityTemplate->getParameters().end();
}

}
