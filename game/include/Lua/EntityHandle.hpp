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
    EntityHandle();
    EntityHandle(entityx::Entity entity);

    components::Component* getComponent(const std::string& componentName);

    std::string getAttributeAsString(const std::string& componentName, const std::string& attributeName) const;
    void setAttributeAsString(const std::string& componentName, const std::string& attributeName, const std::string& value);

    void writeToConsole(const std::string& str);

    void removeEntity();

    static void registerClass(LuaState &state);

private:
    template<class C>
    std::string doGetAttributeAsString(const std::string& attributeName) const
    {
        if(m_entity.has_component<C>())
        {
            auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
                meta::MetadataStore::getMetadata<C>()
            );
            return metadata.getAttribute(attributeName).getAsString(m_entity.component<const C>().get());
        }
        else
        {
            std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
            return "";
        }
    }

    template<class C>
    void doSetAttributeAsString(const std::string& attributeName, const std::string& value)
    {
        if(m_entity.has_component<C>())
        {
            auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
                meta::MetadataStore::getMetadata<C>()
            );
            metadata.getAttribute(attributeName).setAsString(m_entity.component<C>().get(), value);
        }
        else
        {
            std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
        }
    }

    entityx::Entity m_entity;
};

}

#endif
