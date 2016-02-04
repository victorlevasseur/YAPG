#ifndef YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATA_H
#define YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATA_H

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

private:
    T C::*m_member;
};

}

#endif
