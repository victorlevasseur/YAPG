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
        setAsStringImpl(object, value);
    }

    virtual bool getAsBool(const C* object) const
    {
        return getAsBoolImpl(object);
    }

    virtual void setAsBool(C* object, bool value) const
    {
        setAsBoolImpl(object, value);
    }

private:
    /////////////////////// getAsStringImpl /////////////////////////////////////////////////////////////////////////////////
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

    ///////////////////// setAsStringImpl /////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_same<U, std::string>::value, void>::type setAsStringImpl(C* object, const std::string& value) const
    {
        object->*m_member = value;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_same<U, std::string>::value, void>::type setAsStringImpl(C* object, const std::string& value) const
    {
        std::cout << "Script trying to set a value as std::string but the value is not a std::string !" << std::endl;
    }

    ///////////////////// getAsBoolImpl ///////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_convertible<U, bool>::value, bool>::type getAsBoolImpl(const C* object) const
    {
        return object->*m_member;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_convertible<U, bool>::value, bool>::type getAsBoolImpl(const C* object) const
    {
        std::cout << "Script trying to get a value not convertible to bool !" << std::endl;
        return false;
    }

    ///////////////////// setAsBoolImpl ///////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_constructible<U, bool>::value, void>::type setAsBoolImpl(C* object, bool value) const
    {
        object->*m_member = value;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_constructible<U, bool>::value, void>::type setAsBoolImpl(C* object, bool value) const
    {
        std::cout << "Script trying to set a value as bool but the value is not assignable from bool !" << std::endl;
    }

    /**
     * Pointer to the class attribute
     */
    T C::*m_member;
};

}

#endif
