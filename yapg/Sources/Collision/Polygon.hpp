#ifndef YAPG_GAME_TOOLS_POLYGON_HPP
#define YAPG_GAME_TOOLS_POLYGON_HPP

#include <vector>
#include <initializer_list>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf{ class RenderTarget; }
namespace lua{ class LuaState; }

namespace collision
{

class Polygon
{
public:
    Polygon();
    Polygon(std::initializer_list<sf::Vector2f> vertices);
    Polygon(std::vector<sf::Vector2f> vertices);

    ~Polygon();

    sf::Vector2f getPoint(std::size_t index) const;
    void setPoint(std::size_t index, sf::Vector2f position);
    std::size_t getPointsCount() const;

    sf::Vector2f getEdge(std::size_t index) const;

    sf::Vector2f getCenter() const;

    sf::FloatRect getAABB() const;

    sf::Vector2f getTransformedPoint(std::size_t index, sf::Transform transform) const;
    sf::Vector2f getTransformedEdge(std::size_t index, sf::Transform transform) const;

    void drawDebugPolygon(sf::RenderTarget &target);

    static bool collides(Polygon &p1, Polygon &p2, sf::Transform p1Transform = sf::Transform(), sf::Transform p2Transform = sf::Transform());

    static Polygon Rectangle(float width, float height);

    static void registerClass(lua::LuaState& state);

private:
    //Relative vertices positions
    std::vector<sf::Vector2f> m_vertices;
};

}

#endif
