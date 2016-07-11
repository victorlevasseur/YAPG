#ifndef YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATA_H
#define YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATA_H

#include <type_traits>

#include "Lua/sol.hpp"
#include "Meta/Details/AttributeMetadataBase.hpp"
#include "Meta/Details/MetadataStore.hpp"

namespace yapg
{

namespace meta
{

template<class C, typename T>
class AttributeMetadata : public AttributeMetadataBase
{
public:
    AttributeMetadata(
        T C::*member,
        T(C::*getter)() const = nullptr,
        void(C::*setter)(T) = nullptr,
        bool loadableFromLua = true,
        bool gettableFromLua = true,
        bool settableFromLua = true
        ) :
        AttributeMetadataBase(loadableFromLua, gettableFromLua, settableFromLua),
        m_member(member),
        m_getter(getter),
        m_setter(setter)
    {

    }

    virtual std::type_index getType() const override
    {
        return typeid(T);
    }

    virtual void load(void* object_, const sol::object& luaObject) const override
    {
        if(!AttributeMetadataBase::m_loadableFromLua)
            return;

        C* object = reinterpret_cast<C*>(object_);
        //Get the metadata of the class/type to be able to load the attribute
        MetadataStore::getMetadata<T>().load(&(object->*m_member), luaObject);
    }

    virtual void loadFromXml(void* object_, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter) const override
    {
        C* object = reinterpret_cast<C*>(object_);
        //Get the metadata of the class/type to be able to load the attribute from XML according to its meta definition
        MetadataStore::getMetadata<T>().loadFromXml(&(object->*m_member), xmlElement, entityGetter);
    }

    virtual void saveToXml(const void* object_, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter) const override
    {
        const C* object = reinterpret_cast<const C*>(object_);
        //Get the metadata of the class/type to be able to save the attribute to XML according to its meta definition
        MetadataStore::getMetadata<T>().saveToXml(&(object->*m_member), xmlElement, entityGetter);
    }

    virtual boost::any getAsAny(const void* object) const override
    {
        return getAsAnyImpl(reinterpret_cast<const C*>(object));
    }

    virtual void setAsAny(void* object, const boost::any& value) const override
    {
        setAsAnyImpl(reinterpret_cast<C*>(object), value);
    }

private:
    /////////////////// getAsAnyImpl ///////////////////////////////////////////////////////////////////////////////////////////
    template<typename U = T>
    typename std::enable_if<std::is_copy_constructible<U>::value && std::is_nothrow_destructible<U>::value, boost::any>::type getAsAnyImpl(const C* object) const
    {
        return boost::any(getMemberValue(object));
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
            setMemberValue(object, boost::any_cast<U>(value));
        else
            std::cout << "Script trying to set a value as boost::any but with an invalid type !" << std::endl;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_copy_constructible<U>::value || !std::is_nothrow_destructible<U>::value, void>::type setAsAnyImpl(C* object, const boost::any& value) const
    {
        std::cout << "Script to set a value as boost::any but the value is not supported by boost::any (not copy constructible or not nothrow destructible) !" << std::endl;
    }

    T getMemberValue(const C* object) const
    {
        if(m_getter != nullptr)
            return (object->*m_getter)();
        else
            return object->*m_member;
    }

    void setMemberValue(C* object, T value) const
    {
        if(m_setter != nullptr)
            return (object->*m_setter)(value);
        else
            object->*m_member = value;
    }

    /**
     * Pointer to the class attribute
     */
    T C::*m_member;
    T(C::*m_getter)() const;
    void(C::*m_setter)(T);
};

}

}

#endif
