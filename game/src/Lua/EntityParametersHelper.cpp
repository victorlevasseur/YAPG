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

double EntityParametersHelper::getDoubleParameter(const std::string& name) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const lua::EntityTemplate::Parameter& parameter = parameters.at(name);
    return lua::EntityHandle(m_entity).getAttributeAsDouble(parameter.component, parameter.attribute);
}

void EntityParametersHelper::setDoubleParameter(const std::string& name, double value) const
{
    auto& parameters = m_entityTemplate->getParameters();

    const lua::EntityTemplate::Parameter& parameter = parameters.at(name);
    lua::EntityHandle(m_entity).setAttributeAsDouble(parameter.component, parameter.attribute, value);
}

}
