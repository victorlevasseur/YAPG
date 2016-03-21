#ifndef YAPG_GAME_TOOLS_POLYGON_HPP
#define YAPG_GAME_TOOLS_POLYGON_HPP

#include <vector>
#include <initializer_list>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf{ class RenderTarget; }
namespace lua{ class LuaState; }

namespace tools
{

class Polygon
{
public:
    Polygon();
    Polygon(std::initializer_list<sf::Vector2f> vertices);
    Polygon(std::vector<sf::Vector2f> vertices);

    ~Polygon();

    void SetOrigin(sf::Vector2f origin) {m_origin = origin;};
    void SetAngle(float angle){m_angle = angle;};

    sf::Vector2f GetOrigin() const {return m_origin;};
    float GetAngle() const {return m_angle;};

    const std::vector<sf::Vector2f>& GetLocalVertices() const {return m_vertices;};
    sf::FloatRect GetLocalBoundingBox() const;

    const std::vector<sf::Vector2f>& GetGlobalVertices() const {return m_globalVertices;};
    const std::vector<sf::Vector2f>& GetGlobalEdges() const {return m_globalEdges;};

    void DrawDebugPolygon(sf::RenderTarget &target);

    void ComputeGlobalVertices();
    void ComputeGlobalEdges(); // Compute global vertices before
    void ComputeGlobalCenter(); // Compute global vertices before

    static sf::Vector2f RotatePoint(sf::Vector2f point, float angle);

    static Polygon Rectangle(float width, float height);

    static void registerClass(lua::LuaState& state);

private:
    //Relative vertices positions
    std::vector<sf::Vector2f> m_vertices;

    //Position and angle
    sf::Vector2f m_origin;
    float m_angle;

    //Global vertices positions and edges(the origin position and the rotation angle can be set)
    std::vector<sf::Vector2f> m_globalVertices;
    std::vector<sf::Vector2f> m_globalEdges;
    sf::Vector2f m_globalCenter;
};

float DotProduct(sf::Vector2f vec1, sf::Vector2f vec2);
sf::Vector2f Normalise(sf::Vector2f vec);
sf::Vector2f ProjectPolygon(sf::Vector2f &axis, Polygon &p);
float IntervalDistance(sf::Vector2f i1, sf::Vector2f i2);

bool PolygonCollision(Polygon &p1, Polygon &p2);

}

#endif
