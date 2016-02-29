#include "Lua/EntityHandle.hpp"

#include <iostream>

#include "Components/Component.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Lua/LuaState.hpp"

namespace lua
{

std::map<std::string, EntityHandle::ComponentAttributesCallbacks>
    EntityHandle::attributesCallbacks =
        std::map<std::string, EntityHandle::ComponentAttributesCallbacks>();

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
    if(attributesCallbacks.count(componentName) > 0)
    {
        (this->*(attributesCallbacks.at(componentName).loadFromXmlCallback))(attributeName, xmlElement, entityGetter);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

boost::any EntityHandle::getAttributeAsAny(const std::string& componentName, const std::string& attributeName) const
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        return (this->*(attributesCallbacks.at(componentName).getAnyCallback))(attributeName);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
        return "";
    }
}

void EntityHandle::setAttributeAsAny(const std::string& componentName, const std::string& attributeName, const boost::any& value)
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        (this->*(attributesCallbacks.at(componentName).setAnyCallback))(attributeName, value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

void EntityHandle::getAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table result) const
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        (this->*(attributesCallbacks.at(componentName).getLuaTableCallback))(attributeName, result);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

void EntityHandle::setAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table value)
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        (this->*(attributesCallbacks.at(componentName).setLuaTableCallback))(attributeName, value);
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

}
