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
namespace lua
{

class LoaderMetadata
{
public:
    LoaderMetadata()
    {

    };

    virtual ~LoaderMetadata() {};

    virtual void load(void* object, const sol::object& luaObject) const = 0;

private:
};

template<class C, typename T>
class AttributeLoader;

template<class C>
class TypeLoaderMetadata : public LoaderMetadata
{
public:
    TypeLoaderMetadata() :
        LoaderMetadata(),
        m_properties(),
        m_extraFunction()
    {

    }

    virtual void load(void* object, const sol::object& luaObject) const
    {
        loadImpl(reinterpret_cast<C*>(object), luaObject);
    }

    template<typename T, typename D = std::enable_if<std::is_class<C>::value, C>>
    TypeLoaderMetadata<C>& declareLoadableAttribute(const std::string& name, T D::*member)
    {
        m_properties.emplace(name, std::unique_ptr<LoaderMetadata>(new AttributeLoader<D, T>(member)));
        return *this;
    }

    TypeLoaderMetadata<C>& setExtraLoadFunction(std::function<void(C*, const sol::object&)> extraFunction)
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

    std::map<std::string, std::unique_ptr<LoaderMetadata>> m_properties;
    std::function<void(C*, const sol::object&)> m_extraFunction;
};

class MetadataStore
{
public:
    template<class C>
    static TypeLoaderMetadata<C>& registerType()
    {
        metadatas.emplace(std::type_index(typeid(C)), std::unique_ptr<LoaderMetadata>(new TypeLoaderMetadata<C>()));
        LoaderMetadata& to_return = *metadatas[std::type_index(typeid(C))];
        return dynamic_cast<TypeLoaderMetadata<C>&>(to_return);
    }

    template<class C>
    static TypeLoaderMetadata<C>& registerLuaAssignableType()
    {
        return registerType<C>().setExtraLoadFunction(
            [](C* value, const sol::object& luaObject) { *value = luaObject.as<C>(); }
        );
    }

    template<class C>
    static LoaderMetadata& getMetadata()
    {
        if(metadatas.count(std::type_index(typeid(C))) == 0)
        {
            throw std::runtime_error(std::string("Trying to load ") + std::type_index(typeid(C)).name() + std::string(", which is not declared to MetadataStore !"));
        }
        return *metadatas[std::type_index(typeid(C))];
    }

    static LoaderMetadata& getMetadata(std::type_index typeindex)
    {
        if(metadatas.count(typeindex) == 0)
        {
            throw std::runtime_error(std::string("Trying to load ") + typeindex.name() + std::string(", which is not declared to MetadataStore !"));
        }
        return *metadatas[typeindex];
    }

private:
    static std::map<std::type_index, std::unique_ptr<LoaderMetadata>> metadatas;
};

template<class C, typename T>
class AttributeLoader : public LoaderMetadata
{
public:
    AttributeLoader(T C::*member) :
        LoaderMetadata(),
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
