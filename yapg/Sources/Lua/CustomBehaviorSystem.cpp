#include "Lua/CustomBehaviorSystem.hpp"

#include "Lua/CustomBehaviorComponent.hpp"
#include "Lua/EntityHandle.hpp"




namespace yapg
{

CustomBehaviorSystem::CustomBehaviorSystem()
{

}

void CustomBehaviorSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<CustomBehaviorComponent>([&](entityx::Entity entity, CustomBehaviorComponent& comp) {
        comp.updateFunction.call(EntityHandle(entity), (float)dt);
    });
}

}
