#ifndef YAPG_GAME_LUA_ENTITYPARAMETERSHELPER_H
#define YAPG_GAME_LUA_ENTITYPARAMETERSHELPER_H

#include <map>
#include <string>
#include <typeindex>

#include <boost/any.hpp>

#include "entityx/entityx.h"

#include "Template/EntityTemplate.hpp"

namespace yapg{ class EntityHandle; }

namespace yapg
{

/**
 * Utility class allowing to easily get and set the parameters of an entity (according to
 * the parameters defined in its template).
 *
 * It avoids to write code to get the parameters's component and attribute and then
 * get or set it using EntityHandle (a very repetitive code).
 */
class EntityParametersHelper
{
    friend class EntityTemplate;

public:
    EntityParametersHelper(const EntityParametersHelper&) = default;
    EntityParametersHelper& operator=(const EntityParametersHelper&) = default;

    EntityParametersHelper(EntityParametersHelper&&) = default;
    EntityParametersHelper& operator=(EntityParametersHelper&&) = default;

    boost::any getParameter(const std::string& name) const;
    void setParameter(const std::string& name, const boost::any& value) const;
    std::type_index getParameterType(const std::string& name) const;

    std::map<std::string, EntityTemplate::Parameter>::const_iterator parametersBegin() const;
    std::map<std::string, EntityTemplate::Parameter>::const_iterator parametersEnd() const;

private:
    EntityParametersHelper(const EntityTemplate* entityTemplate, entityx::Entity entity);

    const EntityTemplate* m_entityTemplate;
    entityx::Entity m_entity;
};

}

#endif
