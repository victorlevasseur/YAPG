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

    virtual boost::any getAsAny(const C* object) const
    {
        return getAsAnyImpl(object);
    }

    virtual void setAsAny(C* object, const boost::any& value)
    {
        setAsAnyImpl(object, value);
    }

private:
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
