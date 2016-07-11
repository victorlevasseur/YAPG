#include "Lua/Template/EntityParametersHelper.hpp"

#include "Lua/CustomDataComponent.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/Template/EntityTemplate.hpp"

namespace yapg
{

EntityParametersHelper::EntityParametersHelper(const EntityTemplate* entityTemplate, entityx::Entity entity) :
    m_entityTemplate(entityTemplate),
    m_entity(entity)
{

}

boost::any EntityParametersHelper::getParameter(const std::string& name) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        return EntityHandle(m_entity).getAttributeAsAny(parameter.component, parameter.attribute);
    }
    else
    {
        if(EntityHandle(m_entity).getCustomData()->hasValue(parameter.field))
            return EntityHandle(m_entity).getCustomData()->getValue(parameter.field);
        else
            return boost::any();
    }
}

void EntityParametersHelper::setParameter(const std::string& name, const boost::any& value) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        EntityHandle(m_entity).setAttributeAsAny(parameter.component, parameter.attribute, value);
    }
    else
    {
        if(EntityHandle(m_entity).getCustomData()->hasValue(parameter.field))
            EntityHandle(m_entity).getCustomData()->setValue(parameter.field, value);
    }
}

std::type_index EntityParametersHelper::getParameterType(const std::string& name) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        return EntityHandle(m_entity).getAttributeType(parameter.component, parameter.attribute);
    }
    else
    {
        if(EntityHandle(m_entity).getCustomData()->hasValue(parameter.field))
            return EntityHandle(m_entity).getCustomData()->getValue(parameter.field).type();
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
