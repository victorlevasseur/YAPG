#include "Components/TemplateComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

TemplateComponent::TemplateComponent(entityx::Entity entity, int serializedId, const std::string& templateName, const lua::EntityParametersHelper& parametersHelper) :
    Component(entity),
    serializedId(serializedId),
    templateName(templateName),
    parametersHelper(parametersHelper)
{

}

TemplateComponent::~TemplateComponent()
{

}

std::string TemplateComponent::getName() const
{
    return "Template";
}

void TemplateComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<TemplateComponent>("TemplateComponent");
}

std::ostream& operator<<(std::ostream& stream, const TemplateComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
