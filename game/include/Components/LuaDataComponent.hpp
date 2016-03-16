#ifndef YAPG_GAME_COMPONENTS_LUADATACOMPONENT_H
#define YAPG_GAME_COMPONENTS_LUADATACOMPONENT_H

#include <map>

#include <boost/any.hpp>

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class LuaDataComponent : public Component
{
public:
    LuaDataComponent();
    virtual ~LuaDataComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    bool hasValue(const std::string& key) const;
    boost::any getValue(const std::string& key) const;
    void setValue(const std::string& key, const boost::any& value);

    std::map<std::string, boost::any> m_dataTable;
};

std::ostream& operator<<(std::ostream& stream, const LuaDataComponent& component);

}

#endif
