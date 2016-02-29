#include "Lua/EntityParametersHelper.hpp"

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
    return lua::EntityHandle(m_entity).getAttributeAsAny(parameter.component, parameter.attribute);
}

void EntityParametersHelper::setParameter(const std::string& name, const boost::any& value) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const lua::EntityTemplate::Parameter& parameter = parameters.at(name);
    lua::EntityHandle(m_entity).setAttributeAsAny(parameter.component, parameter.attribute, value);
}

}
