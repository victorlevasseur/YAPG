#ifndef YAPG_GAME_META_DETAILS_MAPATTRIBUTEMETADATA_H
#define YAPG_GAME_META_DETAILS_MAPATTRIBUTEMETADATA_H

#include <map>

#include "Lua/sol.hpp"
#include "Meta/Details/AttributeMetadataBase.hpp"
#include "Meta/Details/MetadataStore.hpp"

namespace meta
{

/**
 * Used to declare a std::map<T, U> attribute.
 * Note: T and U must have a default constructor and be (at least) movable.
 */
template<class C, typename T, typename U>
class MapAttributeMetadata : public AttributeMetadataBase<C>
{
public:
    MapAttributeMetadata(std::map<T, U> C::*mapMember) :
        AttributeMetadataBase<C>(),
        m_mapMember(mapMember)

    {

    }

    virtual ~MapAttributeMetadata() {};

    virtual void load(C* object, const sol::object& luaObject) const
    {
        if(!luaObject.is<sol::table>())
            return;

        const sol::table& table = luaObject.as<sol::table>();

        ((*object).*m_mapMember).clear();
        table.for_each([&](const sol::object& key, const sol::object& value) {
            T loadedKey;
            U loaderValue;

            MetadataStore::getMetadata<T>().load(&loadedKey, key);
            MetadataStore::getMetadata<U>().load(&loaderValue, value);

            ((*object).*m_mapMember).emplace(std::make_pair(
                std::move(loadedKey),
                std::move(loaderValue)
            ));
        });
    }

private:
    std::map<T, U> C::*m_mapMember;
};

}

#endif
