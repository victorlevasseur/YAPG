#include "Components/HitboxComponent.hpp"

namespace game
{
namespace components
{

HitboxComponent::HitboxComponent() :
    Component()
{

}

HitboxComponent::~HitboxComponent()
{

}

std::string HitboxComponent::getName() const
{
    return "Hitbox";
}

void HitboxComponent::registerComponent(lua::LuaState& state)
{
    meta::MetadataStore::registerType<HitboxComponent>()
        .setExtraLoadFunction([](HitboxComponent* c, const sol::object& luaObject) {
            const sol::table& polygonTable = luaObject.as<sol::table>().get<sol::table>("polygon");
            std::vector<sf::Vector2f> polygonVertices{};
            polygonTable.for_each([&](const sol::object& key, const sol::object& value) {
                const sol::table& vertexTable = value.as<sol::table>();
                polygonVertices.emplace_back(
                    vertexTable.get<float>("x"),
                    vertexTable.get<float>("y")
                );
            });

            c->getHitbox() = tools::Polygon(polygonVertices);
            c->getHitbox().ComputeGlobalVertices();
            c->getHitbox().ComputeGlobalEdges();
            c->getHitbox().ComputeGlobalCenter();
        });

    state.getState().new_usertype<HitboxComponent>("hitbox_component" //TODO: Replace the name here
        //TODO: Register the properties here
    );
}

std::ostream& operator<<(std::ostream& stream, const HitboxComponent& component)
{
    operator<<(stream, dynamic_cast<const Component&>(component));
    //TODO: Fill with specific infos
    return stream;
}

}
}
