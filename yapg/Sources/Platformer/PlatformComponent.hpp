#ifndef YAPG_GAME_COMPONENTS_PLATFORMCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLATFORMCOMPONENT_H

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"

namespace yapg
{

class PlatformComponent : public Component
{
public:
    enum PlatformType
    {
        Platform = 1,
        Jumpthru = 2,
        All = Platform|Jumpthru
    };

    PlatformComponent(entityx::Entity entity);
    virtual ~PlatformComponent();

    virtual std::string getName() const;

    static void registerComponent(LuaState& state);

    std::string getPlatformTypeAsString() const;
    void setPlatformTypeAsString(const std::string str);

    bool activated;
    PlatformType platformType;
    int layer;

    sol::function onHitFunc;
};

std::ostream& operator<<(std::ostream& stream, const PlatformComponent& component);

}

#endif
