#include "Components/TemplateComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

TemplateComponent::TemplateComponent(int serializedId, const std::string& templateName) :
    Component(),
    serializedId(serializedId),
    templateName(templateName)
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
    meta::MetadataStore::registerClass<TemplateComponent>();
}

std::ostream& operator<<(std::ostream& stream, const TemplateComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
