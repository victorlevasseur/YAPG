#ifndef YAPG_GAME_COMPONENTS_LUADATACOMPONENT_H
#define YAPG_GAME_COMPONENTS_LUADATACOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class LuaDataComponent : public Component
{
public:
    LuaDataComponent(lua_State* L);
    virtual ~LuaDataComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

private:
    sol::table m_luaDataTable;
};

std::ostream& operator<<(std::ostream& stream, const LuaDataComponent& component);

}

#endif
