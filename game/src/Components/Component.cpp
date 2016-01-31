#include "Components/Component.hpp"

#include "Components/CustomBehaviorComponent.hpp"
#include "Components/PlatformComponent.hpp"
#include "Components/PlatformerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Lua/Loader.hpp"

namespace game
{
namespace components
{

Component::Component()
{

}

Component::~Component()
{

}

void Component::loadFromLua(const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    lua::MetadataStore::getMetadata(std::type_index(typeid(*this))).load(this, luaTable);
}

void Component::assignComponent(entityx::Entity entity, const std::string &component, const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    if(component == "Position")
    {
        entity.assign<PositionComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "CustomBehavior")
    {
        entity.assign<CustomBehaviorComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "Platform")
    {
        entity.assign<PlatformComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "Platformer")
    {
        entity.assign<PlatformerComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "Render")
    {
        entity.assign<RenderComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else
    {
        std::cout << "Unknown component of type \"" << component << "\" !" << std::endl;
    }
}

std::ostream& operator<<(std::ostream& stream, const Component& component)
{
    stream << "Component [" << component.getName() << "]" << std::endl;
    return stream;
}

}
}
