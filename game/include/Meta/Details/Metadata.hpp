#ifndef YAPG_GAME_META_DETAILS_METADATA_H
#define YAPG_GAME_META_DETAILS_METADATA_H

#include "Lua/sol.hpp"

namespace meta
{

class Metadata
{
public:
    Metadata()
    {

    };

    virtual ~Metadata() {};

    virtual void load(void* object, const sol::object& luaObject) const = 0;

private:
};

}

#endif
