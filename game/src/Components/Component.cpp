#include "Components/Component.hpp"

#include "Components/CollidableComponent.hpp"
#include "Components/ColliderComponent.hpp"
#include "Components/CustomBehaviorComponent.hpp"
#include "Components/FinishLineComponent.hpp"
#include "Components/HitboxComponent.hpp"
#include "Components/PlatformComponent.hpp"
#include "Components/PlatformerComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/RenderComponent.hpp"
#include "Meta/Metadata.hpp"
#include "Lua/LuaState.hpp"

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
    meta::MetadataStore::getMetadata(std::type_index(typeid(*this))).load(this, luaTable);
}

void Component::assignComponent(entityx::Entity entity, const std::string &component, const sol::object& luaTable, const level::SerializedEntityGetter& entityGetter)
{
    if(component == "position")
    {
        entity.assign<PositionComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "custom_behavior")
    {
        entity.assign<CustomBehaviorComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "hitbox")
    {
        entity.assign<HitboxComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "platform")
    {
        entity.assign<PlatformComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "platformer")
    {
        entity.assign<PlatformerComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "player")
    {
        entity.assign<PlayerComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "render")
    {
        entity.assign<RenderComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "custom_data")
    {
        //Don't do anything, it's managed by initializeEntity from EntityTemplate
    }
    else if(component == "collidable")
    {
        entity.assign<CollidableComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "collider")
    {
        entity.assign<ColliderComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "finish_line")
    {
        entity.assign<FinishLineComponent>().get()->loadFromLua(luaTable, entityGetter);
    }
    else
    {
        std::cout << "Unknown component of type \"" << component << "\" !" << std::endl;
    }
}

void Component::registerComponent(lua::LuaState& state)
{
    state.getState().new_usertype<Component>("component"
    );
}

std::ostream& operator<<(std::ostream& stream, const Component& component)
{
    stream << "Component [" << component.getName() << "]" << std::endl;
    return stream;
}

}
