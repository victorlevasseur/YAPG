#ifndef YAPG_GAME_META_DETAILS_TYPEMETADATA_H
#define YAPG_GAME_META_DETAILS_TYPEMETADATA_H

#include "Level/SerializedEntityGetter.hpp"
#include "Lua/sol.hpp"
#include "Meta/Details/Metadata.hpp"
#include "Settings/tinyxml2.h"

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
        }),
        m_loadXmlFunction()
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

    virtual void loadFromXml(void* value, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const
    {
        if(m_loadXmlFunction)
            m_loadXmlFunction(reinterpret_cast<T*>(value), xmlElement, entityGetter);
        else
            std::cout << "[Meta/Warning] " << typeid(T).name() << " type does not support being loaded from XML !" << std::endl;
    }

    TypeMetadata<T>& setXmlLoadFunction(std::function<void(T*, const tinyxml2::XMLElement*, const level::SerializedEntityGetter&)> loadXmlFunction)
    {
        m_loadXmlFunction = loadXmlFunction;
        return *this;
    }

private:
    std::function<void(T*, const sol::object&)> m_loadFunction;
    std::function<void(T*, const tinyxml2::XMLElement*, const level::SerializedEntityGetter&)> m_loadXmlFunction;
};

}

#endif
