#ifndef YAPG_GAME_LUA_ENTITYHANDLE_H
#define YAPG_GAME_LUA_ENTITYHANDLE_H

#include <typeindex>
#include <type_traits>

#include <boost/any.hpp>

#include "entityx/entityx.h"

#include "Common/Component.hpp"
#include "Lua/sol.hpp"
#include "Meta/Metadata.hpp"

namespace yapg{ class Component; }
namespace yapg{ class CustomDataComponent; }
namespace yapg{ class LuaState; }


namespace yapg
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
        void (EntityHandle::*loadFromXmlCallback)(const std::string&, const tinyxml2::XMLElement*, const SerializedEntityGetter&);

        boost::any (EntityHandle::*getAnyCallback)(const std::string&) const;
        void (EntityHandle::*setAnyCallback)(const std::string&, const boost::any&);

        void (EntityHandle::*getLuaTableCallback)(const std::string&, sol::table) const;
        void (EntityHandle::*setLuaTableCallback)(const std::string&, sol::table);
    };

    EntityHandle();
    EntityHandle(entityx::Entity entity);

    void loadAttributeFromXml(const std::string& componentName, const std::string& attributeName, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter);
    void saveAttributeToXml(const std::string& componentName, const std::string& attributeName, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter) const;

    bool hasComponent(const std::string& componentName) const;
    Component* getComponentPtr(const std::string& componentName);
    const Component* getComponentPtr(const std::string& componentName) const;
    std::type_index getComponentType(const std::string& componentName) const;

    [[deprecated]] boost::any getAttributeAsAny(const std::string& componentName, const std::string& attributeName) const;
    [[deprecated]] void setAttributeAsAny(const std::string& componentName, const std::string& attributeName, const boost::any& value);
    [[deprecated]] std::type_index getAttributeType(const std::string& componentName, const std::string& attributeName) const;

    void getAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table result) const;
    void setAttributeAsLuaTable(const std::string& componentName, const std::string& attributeName, sol::table value);

    CustomDataComponent* getCustomData();

    void writeToConsole(const std::string& str);

    void removeEntity();

    static void registerClass(LuaState &state);

    template<class C>
    static void declareComponent(const std::string& componentName);

    template<class C>
    static C* doGetComponentPtrForLua(EntityHandle entity);

    operator entityx::Entity() { return m_entity; }

private:

    template<class C>
    Component* doGetComponentPtr();

    template<class C>
    const Component* doGetComponentPtrConst() const;

    template<class C>
    bool doHasComponent() const;

    entityx::Entity m_entity;

    static std::map<std::string, std::type_index> componentsTypeIndex;
    static std::map<std::string, std::function<Component*(EntityHandle*)>> componentsGetters;
    static std::map<std::string, std::function<const Component*(const EntityHandle*)>> componentsGettersConst;
    static std::map<std::string, std::function<bool(const EntityHandle*)>> componentsCheckers;
};

}

#include "Lua/EntityHandle.inl"

#endif
