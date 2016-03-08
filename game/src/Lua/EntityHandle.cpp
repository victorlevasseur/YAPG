#include "Lua/EntityHandle.hpp"

#include <iostream>

#include "Components/Component.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Lua/LuaState.hpp"

namespace lua
{

std::map<std::string, std::type_index> EntityHandle::componentsTypeIndex =
        std::map<std::string, std::type_index>();

std::map<std::string, std::function<void*(EntityHandle*)>> EntityHandle::componentsGetters =
        std::map<std::string, std::function<void*(EntityHandle*)>>();

std::map<std::string, std::function<const void*(const EntityHandle*)>> EntityHandle::componentsGettersConst =
        std::map<std::string, std::function<const void*(const EntityHandle*)>>();

std::map<std::string, std::function<bool(const EntityHandle*)>> EntityHandle::componentsCheckers =
        std::map<std::string, std::function<bool(const EntityHandle*)>>();

EntityHandle::EntityHandle() :
    m_entity()
{

}

EntityHandle::EntityHandle(entityx::Entity entity) :
    m_entity(entity)
{

}

void EntityHandle::loadAttributeFromXml(const std::string& componentName, const std::string& attributeName, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter)
{
    if(componentsTypeIndex.count(componentName) > 0 && hasComponent(componentName))
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(componentsTypeIndex.at(componentName));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(attributeName);
        attributeMetadata->loadFromXml(getComponentPtr(componentName), xmlElement, entityGetter);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

void EntityHandle::saveAttributeToXml(const std::string& componentName, const std::string& attributeName, tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const
{
    if(componentsTypeIndex.count(componentName) > 0 && hasComponent(componentName))
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(componentsTypeIndex.at(componentName));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(attributeName);
        attributeMetadata->saveToXml(getComponentPtr(componentName), xmlElement, entityGetter);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

boost::any EntityHandle::getAttributeAsAny(const std::string& componentName, const std::string& attributeName) const
{
    if(componentsTypeIndex.count(componentName) > 0 && hasComponent(componentName))
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(componentsTypeIndex.at(componentName));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(attributeName);
        return attributeMetadata->getAsAny(getComponentPtr(componentName));
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
        return "";
    }
}

void EntityHandle::setAttributeAsAny(const std::string& componentName, const std::string& attributeName, const boost::any& value)
{
    if(componentsTypeIndex.count(componentName) > 0 && hasComponent(componentName))
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(componentsTypeIndex.at(componentName));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(attributeName);
        attributeMetadata->setAsAny(getComponentPtr(componentName), value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

std::type_index EntityHandle::getAttributeType(const std::string& componentName, const std::string& attributeName) const
{
    if(componentsTypeIndex.count(componentName) > 0 && hasComponent(componentName))
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(componentsTypeIndex.at(componentName));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(attributeName);
        return attributeMetadata->getType();
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
        return typeid(void);
    }
}

void EntityHandle::getAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table result) const
{
    if(componentsTypeIndex.count(componentName) > 0 && hasComponent(componentName))
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(componentsTypeIndex.at(componentName));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(attributeName);
        attributeMetadata->getAsLuaTable(getComponentPtr(componentName), result);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

void EntityHandle::setAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table value)
{
    if(componentsTypeIndex.count(componentName) > 0 && hasComponent(componentName))
    {
        const meta::Metadata& componentMetadata = meta::MetadataStore::getMetadata(componentsTypeIndex.at(componentName));
        const meta::AttributeMetadataBase* attributeMetadata = componentMetadata.getAttribute(attributeName);
        attributeMetadata->setAsLuaTable(getComponentPtr(componentName), value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

void EntityHandle::writeToConsole(const std::string& str)
{
    std::cout << str << std::endl;
}

void EntityHandle::removeEntity()
{
    m_entity.destroy();
}

void EntityHandle::registerClass(LuaState &state)
{
    state.getState().new_usertype<EntityHandle>("entity_handle",
        "remove_entity", &EntityHandle::removeEntity,
        "get_attribute", &EntityHandle::getAttributeAsAny,
        "set_attribute", &EntityHandle::setAttributeAsAny,
        "get_table_attribute", &EntityHandle::getAttributeAsLuaTable,
        "set_table_attribute", &EntityHandle::setAttributeAsLuaTable,
        "write_to_console", &EntityHandle::writeToConsole
    );
}

void* EntityHandle::getComponentPtr(const std::string& componentName)
{
    return componentsGetters.at(componentName)(this);
}

const void* EntityHandle::getComponentPtr(const std::string& componentName) const
{
    return componentsGettersConst.at(componentName)(this);
}

bool EntityHandle::hasComponent(const std::string& componentName) const
{
    return componentsCheckers.at(componentName)(this);
}

}
