#ifndef YAPG_GAME_META_DETAILS_MAPATTRIBUTEMETADATA_H
#define YAPG_GAME_META_DETAILS_MAPATTRIBUTEMETADATA_H

#include <map>

#include "Lua/sol.hpp"
#include "Meta/Details/AttributeMetadataBase.hpp"
#include "Meta/Details/MetadataStore.hpp"

namespace meta
{

/**
 * Used to declare a std::map<T, U> attribute. Specialization of AttributeMetadata with T = std::map<U, V>
 * Note: T and U must have a default constructor and be (at least) movable.
 */
template<class C, typename T, typename U>
class AttributeMetadata<C, std::map<T, U>> : public AttributeMetadataBase
{
public:
    AttributeMetadata(
        std::map<T, U> C::*mapMember,
        bool loadableFromLua = true,
        bool gettableFromLua = true,
        bool settableFromLua = true
        ) :
        AttributeMetadataBase(loadableFromLua, gettableFromLua, settableFromLua),
        m_mapMember(mapMember)

    {

    }

    virtual ~AttributeMetadata() {};

    virtual std::type_index getType() const override
    {
        return typeid(std::map<T, U>);
    }

    virtual void load(void* object_, const sol::object& luaObject) const override
    {
        if(!AttributeMetadataBase::m_loadableFromLua)
            return;

        if(!luaObject.is<sol::table>())
            return;

        C* object = reinterpret_cast<C*>(object_);

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

    virtual void loadFromXml(void* object, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const override
    {
        //TODO: Support it !
        std::cout << "[Meta/Warning] Loading std::map attributes from XML is not supported yet !" << std::endl;
    }

    virtual void saveToXml(const void* object, tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const override
    {
        //TODO: Support it !
        std::cout << "[Meta/Warning] Saving std::map attributes to XML is not supported yet !" << std::endl;
    }

    //TODO: Implement get/setAsLuaTable

private:
    std::map<T, U> C::*m_mapMember;
};

}

#endif
