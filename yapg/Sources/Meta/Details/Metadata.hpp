#ifndef YAPG_GAME_META_DETAILS_METADATA_H
#define YAPG_GAME_META_DETAILS_METADATA_H

#include <typeindex>

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

    virtual std::type_index getType() const = 0;

    virtual void load(void* object, const sol::object& luaObject) const = 0;

    virtual void loadFromXml(void* object, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const = 0;

    virtual void saveToXml(const void* object, tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& entityGetter) const = 0;

    /**
     * \return whether the type/class described by this metadata has attributes or not.
     */
    virtual bool hasAttributes() const { return false; }

    /**
     * \param name the attribute's name
     * \return the metadata associated with the attribute named "name"
     */
    virtual const AttributeMetadataBase* getAttribute(const std::string& name) const { return nullptr; }

    /**
     * \param name the attribute's name
     * \return the metadata associated with the attribute named "name"
     */
    virtual AttributeMetadataBase* getAttribute(const std::string& name) { return nullptr; }

private:
};

}

#endif
