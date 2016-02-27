#ifndef YAPG_GAME_COMPONENTS_TEMPLATECOMPONENT_H
#define YAPG_GAME_COMPONENTS_TEMPLATECOMPONENT_H

#include "Components/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class TemplateComponent : public Component
{
public:
    TemplateComponent(int serializedId, const std::string& templateName);
    virtual ~TemplateComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    const int serializedId;
    const std::string templateName;
};

std::ostream& operator<<(std::ostream& stream, const TemplateComponent& component);

}

#endif
