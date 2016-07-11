#include "Collision/Polygon.hpp"

#include <cmath>
#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include "Lua/LuaState.hpp"
#include "Meta/Metadata.hpp"

#define M_PI       3.14159265358979323846


namespace yapg
{

Polygon::Polygon() : m_vertices()
{

}

Polygon::Polygon(std::initializer_list<sf::Vector2f> vertices) : m_vertices()
{
    m_vertices.resize(vertices.size());
    std::copy(vertices.begin(), vertices.end(), m_vertices.begin());
}

Polygon::Polygon(std::vector<sf::Vector2f> vertices) : m_vertices(vertices)
{

}

Polygon::~Polygon()
{

}

sf::Vector2f Polygon::getPoint(std::size_t index) const
{
    return m_vertices[index];
}

void Polygon::setPoint(std::size_t index, sf::Vector2f position)
{
    m_vertices[index] = position;
}

std::size_t Polygon::getPointsCount() const
{
    return m_vertices.size();
}

sf::Vector2f Polygon::getEdge(std::size_t index) const
{
    if(index == getPointsCount() - 1)
        return getPoint(0) - getPoint(index);
    else
        return getPoint(index + 1) - getPoint(index);
}

sf::Vector2f Polygon::getCenter() const
{
    sf::Vector2f sum;
    for(std::size_t i = 0; i < getPointsCount(); ++i)
    {
        sum += getPoint(i);
    }

    return sum / static_cast<float>(getPointsCount());
}

sf::FloatRect Polygon::getAABB() const
{
    if(getPointsCount() == 0)
        return sf::FloatRect();

    sf::Vector2f topLeftMax = getPoint(0);
    sf::Vector2f bottomRightMax = getPoint(0);

    for(std::size_t i = 1; i < getPointsCount(); ++i)
    {
        topLeftMax.x = std::min(getPoint(i).x, topLeftMax.x);
        topLeftMax.y = std::min(getPoint(i).y, topLeftMax.y);
        bottomRightMax.x = std::max(getPoint(i).x, bottomRightMax.x);
        bottomRightMax.y = std::max(getPoint(i).y, bottomRightMax.y);
    }

    return sf::FloatRect(topLeftMax, bottomRightMax - topLeftMax);
}

sf::Vector2f Polygon::getTransformedPoint(std::size_t index, sf::Transform transform) const
{
    return transform.transformPoint(getPoint(index));
}

sf::Vector2f Polygon::getTransformedEdge(std::size_t index, sf::Transform transform) const
{
    if(index == getPointsCount() - 1)
        return transform.transformPoint(getPoint(0)) - transform.transformPoint(getPoint(index));
    else
        return transform.transformPoint(getPoint(index + 1)) - transform.transformPoint(getPoint(index));
}

void Polygon::drawDebugPolygon(sf::RenderTarget &target)
{
    sf::VertexArray poly(sf::LinesStrip);

    for(std::size_t i = 0; i < getPointsCount(); ++i)
    {
        poly.append(sf::Vertex(getPoint(i)));
    }

    poly.append(sf::Vertex(getPoint(0)));

    target.draw(poly);
}

namespace
{
    //Utility functions for the collision test
    float DotProduct(sf::Vector2f vec1, sf::Vector2f vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    sf::Vector2f Normalise(sf::Vector2f vec)
    {
        float length = sqrt(vec.x * vec.x + vec.y * vec.y);

        if(length != 0)
        {
            return vec / length;
        }
        else
        {
            return vec;
        }
    }

    sf::Vector2f ProjectPolygon(sf::Vector2f &axis, Polygon &p, sf::Transform transform)
    {
        sf::Vector2f minMax;
        //First value for the min (x) and max (y)
        minMax.x = minMax.y = DotProduct(axis, p.getTransformedPoint(0, transform));

        for(unsigned int i = 0; i < p.getPointsCount(); i++)
        {
            float dotPr = DotProduct(axis, p.getTransformedPoint(i, transform));
            if(dotPr < minMax.x)
            {
                minMax.x = dotPr;
            }
            else if(dotPr > minMax.y)
            {
                minMax.y = dotPr;
            }
        }

        return minMax;
    }

    float IntervalDistance(sf::Vector2f i1, sf::Vector2f i2)
    {
        if(i1.x < i2.x)
        {
            return i2.x - i1.y;
        }
        else
        {
            return i1.x - i2.y;
        }
    }
}

bool Polygon::collides(Polygon &p1, Polygon &p2, sf::Transform p1Transform, sf::Transform p2Transform)
{
    bool intersect = true;

    std::size_t p1EdgesCount = p1.getPointsCount();
    std::size_t p2EdgesCount = p2.getPointsCount();
    if(p1EdgesCount == 0 || p2EdgesCount == 0)
        return false;

    for(std::size_t i = 0; i < p1EdgesCount + p2EdgesCount; ++i)
    {
        sf::Vector2f edge;

        if(i < p1EdgesCount)
        {
            edge = p1.getTransformedEdge(i, p1Transform);
        }
        else
        {
            edge = p2.getTransformedEdge(i - p1EdgesCount, p2Transform);
        }

        sf::Vector2f axis(-edge.y, edge.x);
        axis = Normalise(axis);

        sf::Vector2f p1Project(ProjectPolygon(axis, p1, p1Transform));
        sf::Vector2f p2Project(ProjectPolygon(axis, p2, p2Transform));

        if(IntervalDistance(p1Project, p2Project) > 0.f)
        {
            intersect = false;
        }
    }

    return intersect;
}

Polygon Polygon::Rectangle(float width, float height)
{
    return Polygon({sf::Vector2f(-width/2, -height/2), sf::Vector2f(+width/2, -height/2), sf::Vector2f(+width/2, +height/2), sf::Vector2f(-width/2, +height/2)});
}

void Polygon::registerClass(LuaState& state)
{
    meta::MetadataStore::registerClass<Polygon>()
        .declareAttribute("points", &Polygon::m_vertices);

    state.getState().new_usertype<Polygon>("polygon",
        "get_local_bounding_box", &Polygon::getAABB
    );

    state.declareAnyConvertibleType<Polygon>("polygon");
}

}
