#include "Collision/PolygonCallback.hpp"

#include "Lua/EntityHandle.hpp"

namespace collision
{

PolygonCallback::PolygonCallback() :
    exclusive(false)
{

}

void PolygonCallback::registerClass(lua::LuaState& state)
{
    meta::MetadataStore::registerClass<PolygonCallback>()
        .declareAttribute("polygon", &PolygonCallback::m_polygon)
        .declareAttribute("on_collision_begin", &PolygonCallback::onCollisionBeginFunc)
        .declareAttribute("collides", &PolygonCallback::collidesFunc)
        .declareAttribute("on_collision_end", &PolygonCallback::onCollisionEndFunc)
        .declareAttribute("exclusive", &PolygonCallback::exclusive);

    state.getState().new_usertype<PolygonCallback>("polygon_callback",
        "polygon", &PolygonCallback::m_polygon
    );

    state.declareAnyConvertibleType<PolygonCallback>("polygon_callback");
}

void PolygonCallback::callOnCollisionBegin(entityx::Entity entity1, entityx::Entity entity2)
{
    if(onCollisionBeginFunc.valid())
        onCollisionBeginFunc.call(lua::EntityHandle(entity1), lua::EntityHandle(entity2));
}

void PolygonCallback::callCollides(entityx::Entity entity1, entityx::Entity entity2)
{
    if(collidesFunc.valid())
        collidesFunc.call(lua::EntityHandle(entity1), lua::EntityHandle(entity2));
}

void PolygonCallback::callOnCollisionEnd(entityx::Entity entity1, entityx::Entity entity2)
{
    if(onCollisionEndFunc.valid())
        onCollisionEndFunc.call(lua::EntityHandle(entity1), lua::EntityHandle(entity2));
}

}
