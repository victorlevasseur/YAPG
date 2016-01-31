#ifndef YAPG_GAME_LUA_LOADER
#define YAPG_GAME_LUA_LOADER

#include <functional>
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
class ClassLoaderMetadata : public LoaderMetadata
{
public:
    ClassLoaderMetadata() :
        LoaderMetadata(),
        m_properties(),
        m_extraFunction()
    {

    }

    virtual void load(void* object, const sol::object& luaObject) const
    {
        loadImpl(reinterpret_cast<C*>(object), luaObject);
    }

    template<typename T>
    ClassLoaderMetadata<C>& declareLoadableAttribute(const std::string& name, T C::*member)
    {
        m_properties.emplace(name, std::unique_ptr<LoaderMetadata>(new AttributeLoader<C, T>(member)));
        return *this;
    }

    ClassLoaderMetadata<C>& setExtraLoadFunction(std::function<void(C*, const sol::object&)> extraFunction)
    {
        m_extraFunction = extraFunction;
        return *this;
    }

private:
    void loadImpl(C* object, const sol::object& luaObject) const
    {
        const sol::table& luaTable = luaObject.as<sol::table>();
        for(auto it = m_properties.cbegin(); it != m_properties.cend(); ++it)
        {
            it->second->load(object, luaTable.get<sol::object>(it->first));
        }

        if(m_extraFunction)
            m_extraFunction(object, luaObject);
    }

    std::map<std::string, std::unique_ptr<LoaderMetadata>> m_properties;
    std::function<void(C*, const sol::object&)> m_extraFunction;
};

class MetadataStore
{
public:
    template<class C>
    static ClassLoaderMetadata<C>& registerClass()
    {
        metadatas.emplace(std::type_index(typeid(C)), std::unique_ptr<LoaderMetadata>(new ClassLoaderMetadata<C>()));
        LoaderMetadata& to_return = *metadatas[std::type_index(typeid(C))];
        return dynamic_cast<ClassLoaderMetadata<C>&>(to_return);
    }

    template<class C>
    static LoaderMetadata& getMetadata()
    {
        return *metadatas[std::type_index(typeid(C))];
    }

    static LoaderMetadata& getMetadata(std::type_index typeindex)
    {
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
        loadImpl(reinterpret_cast<C*>(object), luaObject, std::integral_constant<bool, std::is_arithmetic<T>::value || std::is_same<T, sol::function>::value>());
    }

private:
    /**
     * Version from arithmetic types
     */
    void loadImpl(C* object, const sol::object& luaObject, std::true_type) const
    {
        (*object).*m_member = luaObject.as<T>();
    }

    /**
     * Version for classes, get the class metadata and load it !
     */
    void loadImpl(C* object, const sol::object& luaObject, std::false_type) const
    {
        MetadataStore::getMetadata<T>()->load((*object).*m_member, luaObject);
    }

    T C::*m_member;
};

}
}

#endif
