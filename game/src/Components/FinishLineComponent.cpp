#include "Components/FinishLineComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

FinishLineComponent::FinishLineComponent() :
    Component()
{

}

FinishLineComponent::~FinishLineComponent()
{

}

std::string FinishLineComponent::getName() const
{
    return "FinishLine";
}

void FinishLineComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<FinishLineComponent>();

    lua::EntityHandle::declareComponent<FinishLineComponent>("FinishLine");

    state.getState().new_usertype<FinishLineComponent>("finishline_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
    state.declareComponentGetter<FinishLineComponent>("finish_line");
}

std::ostream& operator<<(std::ostream& stream, const FinishLineComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
