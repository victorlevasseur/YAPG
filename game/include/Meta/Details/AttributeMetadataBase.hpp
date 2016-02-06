#ifndef YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATABASE_H
#define YAPG_GAME_META_DETAILS_ATTRIBUTEMETADATABASE_H

#include "Lua/sol.hpp"

namespace meta
{

template<class C>
class AttributeMetadataBase
{
public:
    AttributeMetadataBase() {};
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
        return "";
    }

    virtual void setAsBool(C* object, bool value) const
    {
        std::cout << "Script trying to set a value not convertible from bool !" << std::endl;
    }
};

}

#endif
