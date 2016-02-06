#ifndef YAPG_GAME_LUA_ENTITYHANDLE_H
#define YAPG_GAME_LUA_ENTITYHANDLE_H

#include <type_traits>

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
        std::string (EntityHandle::*getStringCallback)(const std::string&) const;
        void (EntityHandle::*setStringCallback)(const std::string&, const std::string&);

        bool (EntityHandle::*getBoolCallback)(const std::string&) const;
        void (EntityHandle::*setBoolCallback)(const std::string&, bool);
    };

    EntityHandle();
    EntityHandle(entityx::Entity entity);

    std::string getAttributeAsString(const std::string& componentName, const std::string& attributeName) const;
    void setAttributeAsString(const std::string& componentName, const std::string& attributeName, const std::string& value);

    bool getAttributeAsBool(const std::string& componentName, const std::string& attributeName) const;
    void setAttributeAsBool(const std::string& componentName, const std::string& attributeName, bool value);

    void writeToConsole(const std::string& str);

    void removeEntity();

    static void registerClass(LuaState &state);

    template<class C>
    static void declareComponent(const std::string& componentName);

private:
    template<class C>
    std::string doGetAttributeAsString(const std::string& attributeName) const;

    template<class C>
    void doSetAttributeAsString(const std::string& attributeName, const std::string& value);

    template<class C>
    bool doGetAttributeAsBool(const std::string& attributeName) const;

    template<class C>
    void doSetAttributeAsBool(const std::string& attributeName, bool value);

    entityx::Entity m_entity;

    static std::map<std::string, ComponentAttributesCallbacks> attributesCallbacks;
};

}

#include "Lua/EntityHandle.inl"

#endif
