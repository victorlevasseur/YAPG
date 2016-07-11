#include "Common/Component.hpp"

#include "Common/PositionComponent.hpp"
#include "Collision/CollidableComponent.hpp"
#include "Collision/ColliderComponent.hpp"
#include "Health/HealthComponent.hpp"
#include "Lua/CustomBehaviorComponent.hpp"
#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"
#include "Platformer/PlatformerHitboxComponent.hpp"
#include "Platformer/PlatformComponent.hpp"
#include "Platformer/PlatformerComponent.hpp"
#include "Player/PlayerComponent.hpp"
#include "Rendering/RenderComponent.hpp"

namespace yapg
{

Component::Component(entityx::Entity entity) :
    m_entity(entity)
{

}

Component::~Component()
{

}

void Component::loadFromLua(const sol::object& luaTable, const SerializedEntityGetter& entityGetter)
{
    meta::MetadataStore::getMetadata(std::type_index(typeid(*this))).load(this, luaTable);
}

void Component::assignComponent(entityx::Entity entity, const std::string &component, const sol::object& luaTable, const SerializedEntityGetter& entityGetter)
{
    if(component == "position")
    {
        entity.assign<PositionComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "custom_behavior")
    {
        entity.assign<CustomBehaviorComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "platformer_hitbox")
    {
        entity.assign<PlatformerHitboxComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "platform")
    {
        entity.assign<PlatformComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "platformer")
    {
        entity.assign<PlatformerComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "player")
    {
        entity.assign<PlayerComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "render")
    {
        entity.assign<RenderComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "custom_data")
    {
        //Don't do anything, it's managed by initializeEntity from EntityTemplate
    }
    else if(component == "collidable")
    {
        entity.assign<CollidableComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "collider")
    {
        entity.assign<ColliderComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else if(component == "health")
    {
        entity.assign<HealthComponent>(entity).get()->loadFromLua(luaTable, entityGetter);
    }
    else
    {
        std::cout << "Unknown component of type \"" << component << "\" !" << std::endl;
    }
}

void Component::registerComponent(LuaState& state)
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
