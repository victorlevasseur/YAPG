#ifndef YAPG_GAME_META_DETAILS_VECTORATTRIBUTEMETADATA_H
#define YAPG_GAME_META_DETAILS_VECTORATTRIBUTEMETADATA_H

#include <vector>

#include "Lua/sol.hpp"
#include "Meta/Details/AttributeMetadataBase.hpp"
#include "Meta/Details/MetadataStore.hpp"

namespace meta
{

/**
 * Used to declare a std::vector<T> attribute.
 * Note: T must have a default constructor.
 */
template<class C, typename T>
class VectorAttributeMetadata : public AttributeMetadataBase<C>
{
public:
    VectorAttributeMetadata(std::vector<T> C::*vectorMember) :
        AttributeMetadataBase<C>(),
        m_vectorMember(vectorMember)

    {

    }

    virtual ~VectorAttributeMetadata() {};

    virtual void load(C* object, const sol::object& luaObject) const
    {
        if(!luaObject.is<sol::table>())
            return;

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

private:
    std::vector<T> C::*m_vectorMember;
};

}

#endif
