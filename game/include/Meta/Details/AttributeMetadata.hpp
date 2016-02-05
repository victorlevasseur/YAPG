#ifndef YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATA_H
#define YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATA_H

#include <type_traits>

#include "Lua/sol.hpp"
#include "Meta/Details/AttributeMetadataBase.hpp"
#include "Meta/Details/MetadataStore.hpp"

namespace meta
{

template<class C, typename T>
class AttributeMetadata : public AttributeMetadataBase<C>
{
public:
    AttributeMetadata(T C::*member) :
        AttributeMetadataBase<C>(),
        m_member(member)
    {

    }

    virtual void load(C* object, const sol::object& luaObject) const
    {
        //Get the metadata of the class/type to be able to load the attribute
        MetadataStore::getMetadata<T>().load(&((*object).*m_member), luaObject);
    }

    virtual std::string getAsString(const C* object) const
    {
        return getAsStringImpl(object);
    }

    virtual void setAsString(C* object, const std::string& value) const
    {
        std::cout << "Not implemented !" << std::endl;
        //TODO: Implement this !
    }

private:
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, std::string>::type getAsStringImpl(const C* object) const
    {
        return std::to_string(object->*m_member);
    }

    template<typename U = T>
    typename std::enable_if<std::is_same<U, std::string>::value, std::string>::type getAsStringImpl(const C* object) const
    {
        return object->*m_member;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_arithmetic<U>::value && !std::is_same<U, std::string>::value, std::string>::type getAsStringImpl(const C* object) const
    {
        std::cout << "Script trying to get a value not convertible to std::string !" << std::endl;
        return "";
    }

    T C::*m_member;
};

}

#endif
