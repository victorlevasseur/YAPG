#ifndef YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H
#define YAPG_GAME_COMPONENTS_POSITIONCOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"


namespace components
{

class PositionComponent : public Component
{
public:
    PositionComponent();
    virtual ~PositionComponent();

    virtual std::string getName() const;

    float x;
    float y;
    float width;
    float height;

    static void registerComponent(lua::LuaState& state);
    
protected:
    virtual std::string doGetAttributeAsString(const std::string& attributeName) const;
};

std::ostream& operator<<(std::ostream& stream, const PositionComponent& component);

}

#endif
