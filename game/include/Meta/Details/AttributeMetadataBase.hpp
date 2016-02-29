#ifndef YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATABASE_H
#define YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATABASE_H

#include <boost/any.hpp>

#include "Lua/sol.hpp"

namespace meta
{

template<class C>
class AttributeMetadataBase
{
public:
    AttributeMetadataBase(
        bool loadableFromLua = true,
        bool gettableFromLua = true,
        bool settableFromLua = true
        ) :
        m_loadableFromLua(loadableFromLua),
        m_gettableFromLua(gettableFromLua),
        m_settableFromLua(settableFromLua)
    {

    }

    virtual ~AttributeMetadataBase() {};

    virtual void load(C* object, const sol::object& luaObject) const = 0;

    virtual void loadFromXml(C* object, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const = 0;

    virtual boost::any getAsAny(const C* object) const
    {
        std::cout << "Script trying to get a value not storable in boost::any" << std::endl;
        return boost::any();
    }

    virtual void setAsAny(C* object, const boost::any& value)
    {
        std::cout << "Script trying to set a value not convertible from boost::any" << std::endl;
    }

    /**
     * This is a special one: the table is given as parameter instead of being returned
     * (so that the Metadata don't have to create the table itself,as it would require
     * to depend on a single lua state).
     * Note: sol::table is just a ref to the real lua table, so no need to pass it by
     * reference.
     */
    virtual void getAsLuaTable(const C* object, sol::table result) const
    {
        std::cout << "Script trying to get a value not convertible to a lua table !" << std::endl;
    }

    virtual void setAsLuaTable(C* object, sol::table value) const
    {
        std::cout << "Script trying to set a value not convertible from a lua table !" << std::endl;
    }

protected:
    bool m_loadableFromLua;
    bool m_gettableFromLua;
    bool m_settableFromLua;
};

}

#endif
