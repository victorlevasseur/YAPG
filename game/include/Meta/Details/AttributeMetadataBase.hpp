#ifndef YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATABASE_H
#define YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATABASE_H

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

    virtual std::string getAsString(const C* object) const
    {
        std::cout << "Script trying to get a value not convertible to std::string !" << std::endl;
        return "";
    }

    virtual void setAsString(C* object, const std::string& value) const
    {
        std::cout << "Script trying to set a value not convertible from std::string !" << std::endl;
    }

    virtual bool getAsBool(const C* object) const
    {
        std::cout << "Script trying to get a value not convertible to bool !" << std::endl;
        return false;
    }

    virtual void setAsBool(C* object, bool value) const
    {
        std::cout << "Script trying to set a value not convertible from bool !" << std::endl;
    }

    virtual double getAsDouble(const C* object) const
    {
        std::cout << "Script trying to get a value not convertible to double !" << std::endl;
        return 0.0;
    }

    virtual void setAsDouble(C* object, double value) const
    {
        std::cout << "Script trying to set a value not convertible from double !" << std::endl;
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
