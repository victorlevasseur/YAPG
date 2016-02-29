#ifndef YAPG_GAME_LUA_ENTITYHANDLE_H
#define YAPG_GAME_LUA_ENTITYHANDLE_H

#include <type_traits>

#include <boost/any.hpp>

#include "entityx/entityx.h"

#include "Lua/sol.hpp"
#include "Meta/Metadata.hpp"

namespace components{ class Component; }
namespace lua{ class LuaState; }


namespace lua
{

class EntityHandle
{
public:

    /**
     * Store the callbacks for getting/setting an attribute in a specific
     * component as string, number, bool...
     *
     * Note: the callbacks are specializations of doGet/SetAttributeAsXXX(...)
     * set when EntityHandle::declareComponent<MyComponent> is called
     */
    struct ComponentAttributesCallbacks
    {
        void (EntityHandle::*loadFromXmlCallback)(const std::string&, const tinyxml2::XMLElement*, const level::SerializedEntityGetter&);

        boost::any (EntityHandle::*getAnyCallback)(const std::string&) const;
        void (EntityHandle::*setAnyCallback)(const std::string&, const boost::any&);

        void (EntityHandle::*getLuaTableCallback)(const std::string&, sol::table) const;
        void (EntityHandle::*setLuaTableCallback)(const std::string&, sol::table);
    };

    EntityHandle();
    EntityHandle(entityx::Entity entity);

    void loadAttributeFromXml(const std::string& componentName, const std::string& attributeName, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter);

    boost::any getAttributeAsAny(const std::string& componentName, const std::string& attributeName) const;
    void setAttributeAsAny(const std::string& componentName, const std::string& attributeName, const boost::any& value);

    void getAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table result) const;
    void setAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table value);

    void writeToConsole(const std::string& str);

    void removeEntity();

    static void registerClass(LuaState &state);

    template<class C>
    static void declareComponent(const std::string& componentName);

private:
    template<class C>
    void doLoadAttributeFromXml(const std::string& attributeName, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& serializedEntityGetter);

    template<class C>
    boost::any doGetAttributeAsAny(const std::string& attributeName) const;

    template<class C>
    void doSetAttributeAsAny(const std::string& attributeName, const boost::any& value);

    template<class C>
    void doGetAttributeAsLuaTable(const std::string& attributeName, sol::table result) const;

    template<class C>
    void doSetAttributeAsLuaTable(const std::string& attributeName, sol::table value);

    entityx::Entity m_entity;

    static std::map<std::string, ComponentAttributesCallbacks> attributesCallbacks;
};

}

#include "Lua/EntityHandle.inl"

#endif
