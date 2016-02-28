#ifndef YAPG_GAME_META_DETAILS_METADATA_H
#define YAPG_GAME_META_DETAILS_METADATA_H

#include "Lua/sol.hpp"

namespace level{ class SerializedEntityGetter; }
namespace tinyxml2{ class XMLElement; }

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

    virtual void loadFromXml(void* object, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const = 0;

private:
};

}

#endif
