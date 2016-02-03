#include "Systems/CustomBehaviorSystem.hpp"

#include "Components/CustomBehaviorComponent.hpp"
#include "Lua/EntityHandle.hpp"

namespace c = components;


namespace systems
{

CustomBehaviorSystem::CustomBehaviorSystem()
{

}

void CustomBehaviorSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<c::CustomBehaviorComponent>([&](entityx::Entity entity, c::CustomBehaviorComponent& comp) {
        comp.updateFunction.call(lua::EntityHandle(entity), (float)dt);
    });
}

}
