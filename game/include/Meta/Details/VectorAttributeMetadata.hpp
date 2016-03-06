#ifndef YAPG_GAME_META_DETAILS_VECTORATTRIBUTEMETADATA_H
#define YAPG_GAME_META_DETAILS_VECTORATTRIBUTEMETADATA_H

#include <vector>

#include "Lua/sol.hpp"
#include "Meta/Details/AttributeMetadataBase.hpp"
#include "Meta/Details/MetadataStore.hpp"

namespace meta
{

/**
 * Used to declare a std::vector<T> attribute. Specialization of AttributeMetadata for T <- std::vector<T>
 * Note: T must have a default constructor.
 */
template<class C, typename T>
class AttributeMetadata<C, std::vector<T>> : public AttributeMetadataBase
{
public:
    AttributeMetadata(
        std::vector<T> C::*vectorMember,
        bool loadableFromLua = true,
        bool gettableFromLua = true,
        bool settableFromLua = true
        ) :
        AttributeMetadataBase(loadableFromLua, gettableFromLua, settableFromLua),
        m_vectorMember(vectorMember)

    {

    }

    virtual ~AttributeMetadata() {};

    virtual std::type_index getType() const override
    {
        return typeid(std::vector<T>);
    }

    virtual void load(void* object_, const sol::object& luaObject) const override
    {
        if(!AttributeMetadataBase::m_loadableFromLua)
            return;

        if(!luaObject.is<sol::table>())
            return;

        C* object = reinterpret_cast<C*>(object_);

        const sol::table& table = luaObject.as<sol::table>();

        ((*object).*m_vectorMember).clear();
        table.for_each([&](const sol::object& key, const sol::object& value) {
            ((*object).*m_vectorMember).emplace_back();
            MetadataStore::getMetadata<T>().load(
                &(((*object).*m_vectorMember).back()),
                value
            );
        });
    }

    virtual void loadFromXml(void* object, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const override
    {
        //TODO: Support it !
        std::cout << "[Meta/Warning] Loading std::vector attributes from XML is not supported yet !" << std::endl;
    }

    virtual void saveToXml(const void* object, tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const override
    {
        //TODO: Support it !
        std::cout << "[Meta/Warning] Saving std::vector attributes to XML is not supported yet !" << std::endl;
    }

    /**
     * Return the attribute into "result" as a lua table containing all values of the vector.
     * T must be a type declared to Lua.
     *
     * To support this feature, T must be copyable.
     *
     * Warning: it doesn't convert the vector elements to their serialized
     * form !
     */
    virtual void getAsLuaTable(const void* object, sol::table result) const override
    {
        getAsLuaTableImpl(reinterpret_cast<const C*>(object), result);
    }

    /**
     * Set the vector content from the "value" lua table.
     * Objects inside the table must be convertible to T. For custom class types,
     * it means that they must be declared as a user_type to Lua.
     *
     * To support this feature, T must be assignable.
     *
     * Warning: it doesn't convert the element from their serialized form (on the
     * contrary, that's what loadFromLua does !).
     */
    virtual void setAsLuaTable(void* object, sol::table value) const override
    {
        setAsLuaTableImpl(reinterpret_cast<C*>(object), value);
    }

private:
    template<typename U = T>
    typename std::enable_if<std::is_copy_constructible<U>::value, void>::type
    getAsLuaTableImpl(const C* object, sol::table result) const
    {
        const std::vector<U>& vector = object->*m_vectorMember;
        for(unsigned int i = 0; i < vector.size(); i++)
        {
            result.set(static_cast<int>(i+1), U(vector[i]));
        }
    }

    template<typename U = T>
    typename std::enable_if<!std::is_copy_constructible<U>::value, void>::type
    getAsLuaTableImpl(const C* object, sol::table result) const
    {
        std::cout << "Script trying to get a value not convertible to a lua table !" << std::endl;
    }

    template<typename U = T>
    typename std::enable_if<std::is_copy_assignable<U>::value, void>::type
    setAsLuaTableImpl(C* object, sol::table value) const
    {
        std::vector<U>& vector = object->*m_vectorMember;
        vector.clear();
        value.for_each([&](const sol::object& indexObject, const sol::object& element)
        {
            if(!indexObject.is<int>())
            {
                std::cout << "The lua table used to set a std::vector<T> contains a not numeric key !" << std::endl;
                return;
            }

            unsigned int index = indexObject.as<int>();
            //Resize the vector if needed
            if(vector.size() < index)
                vector.resize(index);
            //Set the value
            vector[index-1] = element.as<U>();
        });
    }

    template<typename U = T>
    typename std::enable_if<!std::is_copy_assignable<U>::value, void>::type
    setAsLuaTableImpl(C* object, sol::table value) const
    {
        std::cout << "Script trying to set a value not convertible from a lua table !" << std::endl;
    }

    std::vector<T> C::*m_vectorMember;
};

}

#endif
