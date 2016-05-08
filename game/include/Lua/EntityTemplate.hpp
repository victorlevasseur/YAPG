#ifndef YAPG_GAME_LUA_ENTITYTEMPLATE_H
#define YAPG_GAME_LUA_ENTITYTEMPLATE_H

#include "entityx/entityx.h"

#include <map>
#include <string>

#include <SFML/Graphics/Image.hpp>

#include "Level/SerializedEntityGetter.hpp"
#include "Lua/sol.hpp"

namespace lua{ class LuaState; }
namespace tinyxml2{ class XMLElement; }

namespace lua
{

class EntityTemplate
{
public:

    struct Parameter
    {
        enum Type
        {
            ComponentAttributeParameter,
            CustomDataFieldParameter
        };

        std::string name;
        std::string friendlyName;

        Type parameterType;

        //Only for ComponentAttributeParameter
        std::string component;
        std::string attribute;

        //Only for CustomDataFieldParameter
        std::string field;
    };

    EntityTemplate(const sol::table& templateTable);
    ~EntityTemplate() = default;

    EntityTemplate(const EntityTemplate&) = delete;
    EntityTemplate& operator=(const EntityTemplate&) = delete;

    EntityTemplate(EntityTemplate&&) = default;
    EntityTemplate& operator=(EntityTemplate&&) = default;

    /**
     * Apply the inheritance to the template (if it has a base template).
     * **Must be called after all template are loaded**.
     */
    void applyInheritance(LuaState& luaState);

    /**
     * Initializes an entity with this template.
     * \param entity the entity to initialize (must be an already created entity)
     * \param entityGetter the entity getter where all entities id are linked to their Entity instance
     * \param templateComponent create or not the TemplateComponent containing infos about which template initialized this entity
     */
    void initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, bool templateComponent = false) const;

    /**
     * Initializes an entity with this template and fulfills its parameters with the parameters in parametersElement XML element.
     * \param entity the entity to initialize (must be an already created entity)
     * \param entityGetter the entity getter where all entities id are linked to their Entity instance
     * \param parametersElement the XML Element containing the values of all the parameters needed by this template
     * \param templateComponent create or not the TemplateComponent containing infos about which template initialized this entity
     */
    void initializeEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, const tinyxml2::XMLElement* parametersElement, bool templateComponent = false) const;

    /**
     * Save the entity parameters in the given parametersElement.
     */
    void saveEntity(entityx::Entity entity, const level::SerializedEntityGetter& entityGetter, tinyxml2::XMLElement* parametersElement) const;

    std::string getName() const { return m_name; }

    std::string getFriendlyName() const { return m_friendlyName; }

    bool isAbstract() const { return m_abstract; }

    bool isPlayer() const;

    sf::Image getImage() const;

    const std::map<std::string, Parameter>& getParameters() const { return m_parameters; }

    const sol::table& getComponentsTable() const { return m_componentsTable; }

private:
    std::string m_name;
    std::string m_friendlyName;

    std::string m_inheritedTemplate;

    bool m_abstract;

    std::map<std::string, Parameter> m_parameters; ///< The parameters of the template

    sol::table m_componentsTable; ///< The "components" table
};

}

#endif
