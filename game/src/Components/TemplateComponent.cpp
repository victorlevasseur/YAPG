#include "Components/TemplateComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

TemplateComponent::TemplateComponent(int serializedId, const std::string& templateName, const lua::EntityParametersHelper& parametersHelper) :
    Component(),
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
    meta::MetadataStore::registerClass<TemplateComponent>("Template");
}

std::ostream& operator<<(std::ostream& stream, const TemplateComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
