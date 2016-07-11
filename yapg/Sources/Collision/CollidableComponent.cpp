#include "Collision/CollidableComponent.hpp"

#include <algorithm>

#include "Lua/EntityHandle.hpp"

namespace yapg
{

CollidableComponent::CollidableComponent(entityx::Entity entity) :
    Component(entity)
{

}

CollidableComponent::~CollidableComponent()
{

}

std::string CollidableComponent::getName() const
{
    return "Collidable";
}

void CollidableComponent::registerComponent(LuaState& state)
{
    meta::MetadataStore::registerClass<CollidableComponent>()
        .declareAttribute("polygons", &CollidableComponent::polygons)
        .setExtraLoadFunction([](CollidableComponent* collidable, const sol::object& luaObject)
        {
            std::copy(
                collidable->polygons.cbegin(),
                collidable->polygons.cend(),
                std::back_inserter(collidable->polygonsByPriority)
            );

            std::sort(
                collidable->polygonsByPriority.begin(),
                collidable->polygonsByPriority.end(),
                [](const std::pair<std::string, PolygonCallback>& p1, const std::pair<std::string, PolygonCallback>& p2)
                {
                    return p1.second.isExclusive() && p2.second.isExclusive();
                }
            );
        });

    EntityHandle::declareComponent<CollidableComponent>("collidable");

    state.getState().new_usertype<CollidableComponent>("collidable_component"
    );
    state.declareComponentGetter<CollidableComponent>("collidable");
}

std::ostream& operator<<(std::ostream& stream, const CollidableComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
