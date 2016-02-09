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

std::string EntityHandle::getAttributeAsString(const std::string& componentName, const std::string& attributeName) const
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        return (this->*(attributesCallbacks.at(componentName).getStringCallback))(attributeName);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
        return "";
    }
}

void EntityHandle::setAttributeAsString(const std::string& componentName, const std::string& attributeName, const std::string& value)
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        (this->*(attributesCallbacks.at(componentName).setStringCallback))(attributeName, value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

bool EntityHandle::getAttributeAsBool(const std::string& componentName, const std::string& attributeName) const
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        return (this->*(attributesCallbacks.at(componentName).getBoolCallback))(attributeName);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
        return false;
    }
}

void EntityHandle::setAttributeAsBool(const std::string& componentName, const std::string& attributeName, bool value)
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        (this->*(attributesCallbacks.at(componentName).setBoolCallback))(attributeName, value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
    }
}

double EntityHandle::getAttributeAsDouble(const std::string& componentName, const std::string& attributeName) const
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        return (this->*(attributesCallbacks.at(componentName).getDoubleCallback))(attributeName);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a not existing component !" << std::endl;
        return false;
    }
}

void EntityHandle::setAttributeAsDouble(const std::string& componentName, const std::string& attributeName, double value)
{
    if(attributesCallbacks.count(componentName) > 0)
    {
        (this->*(attributesCallbacks.at(componentName).setDoubleCallback))(attributeName, value);
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
        "get_string_attribute", &EntityHandle::getAttributeAsString,
        "set_string_attribute", &EntityHandle::setAttributeAsString,
        "get_bool_attribute", &EntityHandle::getAttributeAsBool,
        "set_bool_attribute", &EntityHandle::setAttributeAsBool,
        "get_number_attribute", &EntityHandle::getAttributeAsDouble,
        "set_number_attribute", &EntityHandle::setAttributeAsDouble,
        "get_table_attribute", &EntityHandle::getAttributeAsLuaTable,
        "set_table_attribute", &EntityHandle::setAttributeAsLuaTable,
        "write_to_console", &EntityHandle::writeToConsole
    );
}

}
