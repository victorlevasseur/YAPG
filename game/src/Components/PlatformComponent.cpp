#include "Components/PlatformComponent.hpp"

#include "Lua/EntityHandle.hpp"

namespace components
{

PlatformComponent::PlatformComponent() :
    Component(),
    activated(true),
    platformType(Platform),
    layer(0)
{

}

PlatformComponent::~PlatformComponent()
{

}

std::string PlatformComponent::getName() const
{
    return "Platform";
}

void PlatformComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<PlatformComponent>("PlatformComponent")
        .declareAttribute("activated", &PlatformComponent::activated)
        .setExtraLoadFunction([](PlatformComponent* platform, const sol::object& luaObject)
        {
            const sol::object& platformTypeLua = luaObject.as<sol::table>().get<sol::object>("platform_type");
            if(platformTypeLua.is<std::string>())
            {
                platform->setPlatformTypeAsString(platformTypeLua.as<std::string>());
            }
        });

    lua::EntityHandle::declareComponent<PlatformComponent>("Platform");

    state.getState().new_usertype<PlatformComponent>("platform_component",
        "activated", &PlatformComponent::activated,
        "platform_type", sol::property(&PlatformComponent::getPlatformTypeAsString, &PlatformComponent::setPlatformTypeAsString)
    );
}

std::string PlatformComponent::getPlatformTypeAsString() const
{
    return platformType == PlatformComponent::Platform ? "Platform" : "Jumpthru";
}

void PlatformComponent::setPlatformTypeAsString(const std::string str)
{
    if(str == "Platform")
        platformType = PlatformComponent::Platform;
    else if(str == "Jumpthru")
        platformType = PlatformComponent::Jumpthru;
}

std::ostream& operator<<(std::ostream& stream, const PlatformComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
