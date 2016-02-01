#ifndef YAPG_GAME_LUA_LOADER
#define YAPG_GAME_LUA_LOADER

#include <cassert>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include <type_traits>

#include "Lua/sol.hpp"

namespace game
{
namespace meta
{

class Metadata
{
public:
    Metadata()
    {

    };

    virtual ~Metadata() {};

    virtual void load(void* object, const sol::object& luaObject) const = 0;

private:
};

template<class C, typename T>
class AttributeMetadata;

template<class C>
class TypeMetadata : public Metadata
{
public:
    TypeMetadata() :
        Metadata(),
        m_properties(),
        m_extraFunction()
    {

    }

    virtual void load(void* object, const sol::object& luaObject) const
    {
        loadImpl(reinterpret_cast<C*>(object), luaObject);
    }

    template<typename T, typename D = std::enable_if<std::is_class<C>::value, C>>
    TypeMetadata<C>& declareAttribute(const std::string& name, T D::*member)
    {
        m_properties.emplace(name, std::unique_ptr<Metadata>(new AttributeMetadata<D, T>(member)));
        return *this;
    }

    TypeMetadata<C>& setExtraLoadFunction(std::function<void(C*, const sol::object&)> extraFunction)
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
            for(auto it = m_properties.cbegin(); it != m_properties.cend(); ++it)
            {
                it->second->load(object, luaTable.get<sol::object>(it->first));
            }
        }

        if(m_extraFunction)
        {
            m_extraFunction(object, luaObject);
        }

    }

    std::map<std::string, std::unique_ptr<Metadata>> m_properties;
    std::function<void(C*, const sol::object&)> m_extraFunction;
};

class MetadataStore
{
public:
    template<class C>
    static TypeMetadata<C>& registerType()
    {
        metadatas.emplace(std::type_index(typeid(C)), std::unique_ptr<Metadata>(new TypeMetadata<C>()));
        Metadata& to_return = *metadatas[std::type_index(typeid(C))];
        return dynamic_cast<TypeMetadata<C>&>(to_return);
    }

    template<class C>
    static TypeMetadata<C>& registerLuaAssignableType()
    {
        return registerType<C>().setExtraLoadFunction(
            [](C* value, const sol::object& luaObject) { *value = luaObject.as<C>(); }
        );
    }

    template<class C>
    static Metadata& getMetadata()
    {
        if(metadatas.count(std::type_index(typeid(C))) == 0)
        {
            throw std::runtime_error(std::string("Trying to load ") + std::type_index(typeid(C)).name() + std::string(", which is not declared to MetadataStore !"));
        }
        return *metadatas[std::type_index(typeid(C))];
    }

    static Metadata& getMetadata(std::type_index typeindex)
    {
        if(metadatas.count(typeindex) == 0)
        {
            throw std::runtime_error(std::string("Trying to load ") + typeindex.name() + std::string(", which is not declared to MetadataStore !"));
        }
        return *metadatas[typeindex];
    }

private:
    static std::map<std::type_index, std::unique_ptr<Metadata>> metadatas;
};

template<class C, typename T>
class AttributeMetadata : public Metadata
{
public:
    AttributeMetadata(T C::*member) :
        Metadata(),
        m_member(member)
    {

    }

    virtual void load(void* object, const sol::object& luaObject) const
    {
        //Get the metadata of the class/type to be able to load the attribute
        C* typedObject = reinterpret_cast<C*>(object);
        MetadataStore::getMetadata<T>().load(&((*typedObject).*m_member), luaObject);
    }

private:
    T C::*m_member;
};

}
}

#endif
