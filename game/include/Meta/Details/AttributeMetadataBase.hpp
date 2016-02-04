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
};

}

#endif
