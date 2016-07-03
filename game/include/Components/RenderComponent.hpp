#ifndef YAPG_GAME_COMPONENTS_RENDERCOMPONENT_H
#define YAPG_GAME_COMPONENTS_RENDERCOMPONENT_H

#include <map>
#include <string>

#include "Animation/Animation.hpp"
#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Lua/sol.hpp"
#include "Messaging/Messaging.hpp"

namespace components
{

class RenderComponent : public Component, public messaging::Emitter
{
public:
    RenderComponent(entityx::Entity entity);
    virtual ~RenderComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    std::string textureName;

    std::string currentAnimation;
    std::string getCurrentAnimation() const { return currentAnimation; }
    void setCurrentAnimation(std::string animationName);

    std::map<std::string, animation::Animation> animations;
    bool flipped;

    sol::function onAnimationChangedFunc;
    sol::function onAnimationEndFunc;
};

std::ostream& operator<<(std::ostream& stream, const RenderComponent& component);

}

#endif
