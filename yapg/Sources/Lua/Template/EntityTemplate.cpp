#include "Lua/Template/EntityTemplate.hpp"

#include <SFML/Graphics/Texture.hpp>

#include "Common/Component.hpp"
#include "Lua/CustomDataComponent.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/LuaState.hpp"
#include "Lua/Template/EntityParametersHelper.hpp"
#include "Lua/Template/TemplateComponent.hpp"
#include "Settings/tinyxml2.h"

namespace yapg
{

EntityTemplate::EntityTemplate(const sol::table& templateTable, const std::string& packageName) :
    m_name(),
    m_friendlyName(),
    m_inheritedTemplate(),
    m_abstract(false),
    m_parameters(),
    m_componentsTable(),
    m_packageName(packageName)
{
    m_name = templateTable.get<std::string>("name");
    m_friendlyName = templateTable.get<std::string>("friendlyname");

    if(templateTable.get<sol::object>("inherits").is<std::string>())
        m_inheritedTemplate = templateTable.get<std::string>("inherits");

    m_componentsTable = templateTable.get<sol::table>("components");

    if(templateTable.get<sol::object>("abstract").is<bool>())
        m_abstract = templateTable.get<bool>("abstract");

    //Load the parameters
    if(templateTable.get<sol::object>("parameters").is<sol::table>())
    {
        templateTable.get<sol::table>("parameters").for_each([&](const sol::object &key, const sol::object &value)
        {
            sol::table valueTable = value.as<sol::table>();

            if(valueTable.get<sol::object>("custom_data_field").valid())
            {
                //It's a custom data parameter (a parameter that affects a value to a data stored in CustomDataComponent : can be any type of data)
                m_parameters.emplace(
                    key.as<std::string>(),
                    Parameter{
                        key.as<std::string>(),
                        valueTable.get<std::string>("name"),
                        Parameter::CustomDataFieldParameter,
                        "",
                        "",
                        valueTable.get<std::string>("custom_data_field")
                    }
                );
            }
            else
            {
                //It's a parameter to a component's attribute
                m_parameters.emplace(
                    key.as<std::string>(),
                    Parameter{
                        key.as<std::string>(),
                        valueTable.get<std::string>("name"),
                        Parameter::ComponentAttributeParameter,
                        valueTable.get<std::string>("component"),
                        valueTable.get<std::string>("attribute"),
                        ""
                    }
                );
            }
        });
    }
}

void EntityTemplate::applyInheritance(LuaState& luaState)
{
    if(!m_inheritedTemplate.empty())
    {
        //Merge the parameters
        m_parameters.insert(
            luaState.getTemplate(m_packageName + "." + m_inheritedTemplate).m_parameters.begin(),
            luaState.getTemplate(m_packageName + "." + m_inheritedTemplate).m_parameters.end()
        );

        //Merge the component lua table
        m_componentsTable = luaState.mergeTables(luaState.getTemplate(m_packageName + "." + m_inheritedTemplate).m_componentsTable, m_componentsTable);
    }
}

void EntityTemplate::initializeEntity(entityx::Entity entity, const SerializedEntityGetter& entityGetter) const
{
    if(isAbstract())
        throw std::runtime_error("[Template/Error] Trying to instanciate \"" + getName() + "\", which is an abstract template!");

    //Add each components to the entity
    m_componentsTable.for_each([&](const sol::object& key, const sol::object& value) {
        std::string componentType = key.as<std::string>();

        Component::assignComponent(entity, componentType, value, entityGetter);
    });

    //Add the CustomData component (special case!)
    entity.assign<CustomDataComponent>(entity);
    if(m_componentsTable.get<sol::object>("custom_data").is<sol::table>())
    {
        //If the template has CustomDataComponent defined, it means that he needs to predefine some values
        entity.component<CustomDataComponent>()->loadFromLua(
            m_componentsTable.get<sol::object>("custom_data"),
            entityGetter
        );
    }

    //Add the template component, containing infos about the template
    entity.assign<TemplateComponent>(
        entity,
        entityGetter.getSerializationIdFromEntity(entity),
        getName(),
        m_packageName,
        EntityParametersHelper(this, entity)
    );
}

void EntityTemplate::initializeEntity(entityx::Entity entity, const SerializedEntityGetter& entityGetter, const tinyxml2::XMLElement* parametersElement) const
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

            //Load the corresponding parameter (from parameter infos) using the XML element
            if(it->second.parameterType == Parameter::ComponentAttributeParameter)
            {
                EntityHandle(entity).loadAttributeFromXml(it->second.component, it->second.attribute, parameterElement, entityGetter);
            }
            else
            {
                if(EntityHandle(entity).getCustomData()->hasValue(it->second.field)) //Note: the template must have already defined the custom data value.
                {
                    boost::any value = EntityHandle(entity).getCustomData()->getValue(it->second.field);
                    meta::MetadataStore::getMetadata(value.type()).loadFromXml(boost::unsafe_any_cast<void*>(&value), parameterElement, entityGetter);
                    EntityHandle(entity).getCustomData()->setValue(it->second.field, value);
                }
            }
        }
        catch(std::exception& e)
        {
            std::cout << "[Lua/Warning] " << e.what() << std::endl;
        }
    }
}

void EntityTemplate::saveEntity(entityx::Entity entity, const SerializedEntityGetter& entityGetter, tinyxml2::XMLElement* parametersElement) const
{
    tinyxml2::XMLDocument* doc = parametersElement->GetDocument();

    for(auto it = m_parameters.cbegin(); it != m_parameters.cend(); ++it)
    {
        tinyxml2::XMLElement* parameterElement = doc->NewElement(it->first.c_str());
        parametersElement->LinkEndChild(parameterElement);

        if(it->second.parameterType == Parameter::ComponentAttributeParameter)
        {
            EntityHandle(entity).saveAttributeToXml(it->second.component, it->second.attribute, parameterElement, entityGetter);
        }
        else
        {
            if(EntityHandle(entity).getCustomData()->hasValue(it->second.field))
            {
                boost::any value = EntityHandle(entity).getCustomData()->getValue(it->second.field);
                meta::MetadataStore::getMetadata(value.type()).saveToXml(boost::unsafe_any_cast<void*>(&value), parameterElement, entityGetter);
            }
        }
    }
}

bool EntityTemplate::isPlayer() const
{
    return getComponentsTable().get<sol::object>("player").is<sol::table>();
}

sf::Image EntityTemplate::getImage() const
{
    if(getComponentsTable().get<sol::object>("render").is<sol::table>())
    {
        sol::table renderTable = getComponentsTable().get<sol::table>("render");

        std::string texturePath = "packages/" + m_packageName + "/assets/" + renderTable.get<std::string>("texture");

        std::string defaultAnimationName = renderTable.get<std::string>("current_animation");
        sol::table defaultAnimation = renderTable.get<sol::table>("animations").get<sol::table>(defaultAnimationName);
        sol::table frames = defaultAnimation.get<sol::table>("frames");

        sol::table firstFrame = frames.get<sol::table>(1);
        sol::table rectTable = firstFrame.get<sol::table>("rect");

        sf::IntRect imageRect(
            rectTable.get<int>("left"),
            rectTable.get<int>("top"),
            rectTable.get<int>("width"),
            rectTable.get<int>("height")
        );

        sf::Texture texture;
        if(!texture.loadFromFile(texturePath, imageRect))
        {
            return sf::Image();
        }

        return texture.copyToImage();
    }
    else
    {
        return sf::Image();
    }
}

sf::Texture EntityTemplate::getTexture() const
{
    if(getComponentsTable().get<sol::object>("render").is<sol::table>())
    {
        sol::table renderTable = getComponentsTable().get<sol::table>("render");

        std::string texturePath = "packages/" + m_packageName + "/assets/" + renderTable.get<std::string>("texture");

        std::string defaultAnimationName = renderTable.get<std::string>("current_animation");
        sol::table defaultAnimation = renderTable.get<sol::table>("animations").get<sol::table>(defaultAnimationName);
        sol::table frames = defaultAnimation.get<sol::table>("frames");

        sol::table firstFrame = frames.get<sol::table>(1);
        sol::table rectTable = firstFrame.get<sol::table>("rect");

        sf::IntRect imageRect(
            rectTable.get<int>("left"),
            rectTable.get<int>("top"),
            rectTable.get<int>("width"),
            rectTable.get<int>("height")
        );

        sf::Texture texture;
        if(!texture.loadFromFile(texturePath, imageRect))
        {
            return sf::Texture();
        }

        return texture;
    }
    else
    {
        return sf::Texture();
    }
}

}
