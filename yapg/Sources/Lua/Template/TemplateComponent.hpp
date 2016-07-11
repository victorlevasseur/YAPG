#ifndef YAPG_GAME_COMPONENTS_TEMPLATECOMPONENT_H
#define YAPG_GAME_COMPONENTS_TEMPLATECOMPONENT_H

#include "Common/Component.hpp"
#include "Lua/LuaState.hpp"
#include "Lua/Template/EntityParametersHelper.hpp"
#include "Meta/Metadata.hpp"

namespace components
{

class TemplateComponent : public Component
{
public:
    TemplateComponent(entityx::Entity entity, int serializedId, const std::string& templateName, const lua::EntityParametersHelper& parametersHelper);
    virtual ~TemplateComponent();

    virtual std::string getName() const;

    static void registerComponent(lua::LuaState& state);

    int serializedId;
    const std::string templateName;

    const lua::EntityParametersHelper parametersHelper;
};

std::ostream& operator<<(std::ostream& stream, const TemplateComponent& component);

}

#endif
