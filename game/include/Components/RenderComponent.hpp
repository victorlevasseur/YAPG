#ifndef YAPG_GAME_COMPONENTS_RENDERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_RENDERCOMPONENT_H

#include <map>
#include <string>

#include "Animation/Animation.hpp"
#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"

namespace components
{

class RenderComponent : public Component
{
public:
    RenderComponent();
    virtual ~RenderComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    std::string textureName;
    std::string currentAnimation;
    std::map<std::string, animation::Animation> animations;
    bool flipped;
};

std::ostream& operator<<(std::ostream& stream, const RenderComponent& component);

}

#endif
