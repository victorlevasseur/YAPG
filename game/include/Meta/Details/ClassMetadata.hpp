#ifndef YAPG_GAME_META_DETAILS_CLASSMETADATA_H
#define YAPG_GAME_META_DETAILS_CLASSMETADATA_H

#include "Lua/sol.hpp"
#include "Meta/Details/Metadata.hpp"
#include "Meta/Details/MetadataStore.hpp"

namespace meta
{

template<class C>
class AttributeMetadataBase;

template<class C, typename T>
class AttributeMetadata;

template<class C>
class ClassMetadata : public Metadata
{
public:
    ClassMetadata() :
        Metadata(),
        m_attributes(),
        m_extraFunction()
    {

    }

    virtual void load(void* object, const sol::object& luaObject) const
    {
        loadImpl(reinterpret_cast<C*>(object), luaObject);
    }

    template<typename T, typename D = std::enable_if<std::is_class<C>::value, C>>
    ClassMetadata<C>& declareAttribute(const std::string& name, T D::*member)
    {
        m_attributes.emplace(name, std::unique_ptr<AttributeMetadataBase<D>>(new AttributeMetadata<D, T>(member)));
        return *this;
    }

    ClassMetadata<C>& setExtraLoadFunction(std::function<void(C*, const sol::object&)> extraFunction)
    {
        m_extraFunction = extraFunction;
        return *this;
    }

private:
    void loadImpl(C* object, const sol::object& luaObject) const
    {
        if(luaObject.is<sol::table>())
        {
            const sol::table& luaTable = luaObject.as<sol::table>();
            for(auto it = m_attributes.cbegin(); it != m_attributes.cend(); ++it)
            {
                it->second->load(object, luaTable.get<sol::object>(it->first));
            }
        }

        if(m_extraFunction)
        {
            m_extraFunction(object, luaObject);
        }

    }

    std::map<std::string, std::unique_ptr<AttributeMetadataBase<C>>> m_attributes;
    std::function<void(C*, const sol::object&)> m_extraFunction;
};

}

#endif
