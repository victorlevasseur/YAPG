#include "Template/EntityParametersHelper.hpp"

#include "Lua/CustomDataComponent.hpp"
#include "Lua/EntityHandle.hpp"
#include "Template/EntityTemplate.hpp"

namespace yapg
{

EntityParametersHelper::EntityParametersHelper(const EntityTemplate* entityTemplate, entityx::Entity entity) :
    m_entityTemplate(entityTemplate),
    m_entity(entity)
{

}

void EntityParametersHelper::loadParameter(const std::string& name, const sol::object& object) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(EntityHandle(m_entity).getComponentType(parameter.component));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(parameter.attribute);
        attributeMetadata->load(EntityHandle(m_entity).getComponentPtr(parameter.component), object);
    }
    else
    {
        throw std::runtime_error("[Lua/Error] Can't load a custom data parameter from lua template syntax!");
        /* TODO:
        if(EntityHandle(m_entity).getCustomData()->hasValue(parameter.field))
            return EntityHandle(m_entity).getCustomData()->getValue(parameter.field);
        else
            return boost::any();*/
    }
}

boost::any EntityParametersHelper::getParameter(const std::string& name) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const EntityTemplate::Parameter& parameter = parameters.at(name);
    if(parameter.parameterType == EntityTemplate::Parameter::ComponentAttributeParameter)
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(EntityHandle(m_entity).getComponentType(parameter.component));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(parameter.attribute);
        return attributeMetadata->getAsAny(EntityHandle(m_entity).getComponentPtr(parameter.component));
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
        //EntityHandle(m_entity).setAttributeAsAny(parameter.component, parameter.attribute, value);
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(EntityHandle(m_entity).getComponentType(parameter.component));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(parameter.attribute);
        attributeMetadata->setAsAny(EntityHandle(m_entity).getComponentPtr(parameter.component), value);
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
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(EntityHandle(m_entity).getComponentType(parameter.component));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(parameter.attribute);
        return attributeMetadata->getType();
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
