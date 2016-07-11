#ifndef YAPG_GAME_COMPONENTS_CUSTOMDATACOMPONENT_H
#define YAPG_GAME_COMPONENTS_CUSTOMDATACOMPONENT_H

#include <map>

#include <boost/any.hpp>

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class CustomDataComponent : public Component
{
public:
    CustomDataComponent(entityx::Entity entity);
    virtual ~CustomDataComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    bool hasValue(const std::string& key) const;
    boost::any getValue(const std::string& key) const;
    void setValue(const std::string& key, const boost::any& value);

    std::map<std::string, boost::any> m_dataTable;
};

std::ostream& operator<<(std::ostream& stream, const CustomDataComponent& component);

}

#endif
