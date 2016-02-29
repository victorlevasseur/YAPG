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
    AttributeMetadata(
        T C::*member,
        bool loadableFromLua = true,
        bool gettableFromLua = true,
        bool settableFromLua = true
        ) :
        AttributeMetadataBase<C>(loadableFromLua, gettableFromLua, settableFromLua),
        m_member(member)
    {

    }

    virtual void load(C* object, const sol::object& luaObject) const
    {
        if(!AttributeMetadataBase<C>::m_loadableFromLua)
            return;
        //Get the metadata of the class/type to be able to load the attribute
        MetadataStore::getMetadata<T>().load(&(object->*m_member), luaObject);
    }

    virtual void loadFromXml(C* object, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const
    {
        //Get the metadata of the class/type to be able to load the attribute from XML according to its meta definition
        MetadataStore::getMetadata<T>().loadFromXml(&(object->*m_member), xmlElement, entityGetter);
    }

    virtual std::string getAsString(const C* object) const
    {
        if(!AttributeMetadataBase<C>::m_gettableFromLua)
        {
            std::cout << "Script trying to get a value not visible from lua !" << std::endl;
            return "";
        }

        return getAsStringImpl(object);
    }

    virtual void setAsString(C* object, const std::string& value) const
    {
        if(!AttributeMetadataBase<C>::m_settableFromLua)
        {
            std::cout << "Script trying to set a value not changeable from lua !" << std::endl;
            return;
        }

        setAsStringImpl(object, value);
    }

    virtual bool getAsBool(const C* object) const
    {
        if(!AttributeMetadataBase<C>::m_gettableFromLua)
        {
            std::cout << "Script trying to get a value not visible from lua !" << std::endl;
            return false;
        }

        return getAsBoolImpl(object);
    }

    virtual void setAsBool(C* object, bool value) const
    {
        if(!AttributeMetadataBase<C>::m_settableFromLua)
        {
            std::cout << "Script trying to set a value not changeable from lua !" << std::endl;
            return;
        }

        setAsBoolImpl(object, value);
    }

    virtual double getAsDouble(const C* object) const
    {
        if(!AttributeMetadataBase<C>::m_gettableFromLua)
        {
            std::cout << "Script trying to get a value not visible from lua !" << std::endl;
            return 0.0;
        }

        return getAsDoubleImpl(object);
    }

    virtual void setAsDouble(C* object, double value) const
    {
        if(!AttributeMetadataBase<C>::m_settableFromLua)
        {
            std::cout << "Script trying to set a value not changeable from lua !" << std::endl;
            return;
        }

        setAsDoubleImpl(object, value);
    }

    virtual boost::any getAsAny(const C* object) const
    {
        return getAsAnyImpl(object);
    }

    virtual void setAsAny(C* object, const boost::any& value)
    {
        setAsAnyImpl(object, value);
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

    ///////////////////// getAsDoubleImpl ///////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_convertible<U, double>::value, double>::type getAsDoubleImpl(const C* object) const
    {
        return object->*m_member;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_convertible<U, double>::value, double>::type getAsDoubleImpl(const C* object) const
    {
        std::cout << "Script trying to get a value not convertible to double !" << std::endl;
        return 0.0;
    }

    ///////////////////// setAsDoubleImpl ///////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_constructible<U, double>::value, void>::type setAsDoubleImpl(C* object, double value) const
    {
        object->*m_member = value;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_constructible<U, double>::value, void>::type setAsDoubleImpl(C* object, double value) const
    {
        std::cout << "Script trying to set a value as bool but the value is not assignable from double !" << std::endl;
    }

    /////////////////// getAsAnyImpl ///////////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_copy_constructible<U>::value && std::is_nothrow_destructible<U>::value, boost::any>::type getAsAnyImpl(const C* object) const
    {
        return boost::any(object->*m_member);
    }

    template<typename U = T>
    typename std::enable_if<!std::is_copy_constructible<U>::value || !std::is_nothrow_destructible<U>::value, boost::any>::type getAsAnyImpl(const C* object) const
    {
        std::cout << "Script to get a value as boost::any but the value is not supported by boost::any (not copy constructible or not nothrow destructible) !" << std::endl;
        return boost::any();
    }

    /////////////////// setAsAnyImpl ///////////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_copy_constructible<U>::value && std::is_nothrow_destructible<U>::value, void>::type setAsAnyImpl(C* object, const boost::any& value) const
    {
        if(value.type() == typeid(U))
            object->*m_member = boost::any_cast<U>(value);
        else
            std::cout << "Script trying to set a value as boost::any but with an invalid type !" << std::endl;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_copy_constructible<U>::value || !std::is_nothrow_destructible<U>::value, void>::type setAsAnyImpl(C* object, const boost::any& value) const
    {
        std::cout << "Script to set a value as boost::any but the value is not supported by boost::any (not copy constructible or not nothrow destructible) !" << std::endl;
    }

    /**
     * Pointer to the class attribute
     */
    T C::*m_member;
};

}

#endif
