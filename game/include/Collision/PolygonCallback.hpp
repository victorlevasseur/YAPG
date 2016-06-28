#ifndef YAPG_GAME_COLLISION_POLYGONCALLBACK_H
#define YAPG_GAME_COLLISION_POLYGONCALLBACK_H

#include "Collision/Polygon.hpp"
#include "Collision/PolygonCallback.hpp"
#include "Lua/LuaState.hpp"
#include "Lua/sol.hpp"

namespace collision
{

class PolygonCallback
{
public:
    PolygonCallback();

    static void registerClass(lua::LuaState& state);

    const Polygon& getPolygon() const { return m_polygon; }
    Polygon& getPolygon() { return m_polygon; }

    void callOnCollisionBegin(entityx::Entity entity1, entityx::Entity entity2);
    void callCollides(entityx::Entity entity1, entityx::Entity entity2);
    void callOnCollisionEnd(entityx::Entity entity1, entityx::Entity entity2);

private:
    Polygon m_polygon;

    sol::function onCollisionBeginFunc;
    sol::function collidesFunc;
    sol::function onCollisionEndFunc;

    bool exclusive;
};

}

#endif
