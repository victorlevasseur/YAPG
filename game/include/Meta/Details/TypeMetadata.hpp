#ifndef YAPG_GAME_META_DETAILS_TYPEMETADATA_H
#define YAPG_GAME_META_DETAILS_TYPEMETADATA_H

#include "Lua/sol.hpp"
#include "Meta/Details/Metadata.hpp"

namespace meta
{

template<typename T>
class TypeMetadata : public Metadata
{
public:
    TypeMetadata() :
        Metadata(),
        m_loadFunction([](T* value, const sol::object& luaObject) {
            if(luaObject.is<T>())
                *value = luaObject.as<T>();
        })
    {

    }

    virtual void load(void* value, const sol::object& luaObject) const
    {
        m_loadFunction(reinterpret_cast<T*>(value), luaObject);
    }

    TypeMetadata<T>& setCustomLoadFunction(std::function<void(T*, const sol::object&)> loadFunction)
    {
        m_loadFunction = loadFunction;
        return *this;
    }

private:
    std::function<void(T*, const sol::object&)> m_loadFunction;
};

}

#endif
