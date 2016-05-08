#ifndef YAPG_GAME_COMPONENTS_PLATFORMCOMPONENT_H
#define YAPG_GAME_COMPONENTS_PLATFORMCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"

namespace components
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

    static void registerComponent(lua::LuaState& state);

    std::string getPlatformTypeAsString() const;
    void setPlatformTypeAsString(const std::string str);

    bool activated;
    PlatformType platformType;
    int layer;
};

std::ostream& operator<<(std::ostream& stream, const PlatformComponent& component);

}

#endif
