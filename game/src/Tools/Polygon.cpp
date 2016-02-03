#include "Tools/Polygon.hpp"

#include <cmath>
#include <iostream>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#define M_PI       3.14159265358979323846


namespace tools
{

Polygon::Polygon() : m_vertices(), m_origin(), m_angle(0), m_globalVertices(), m_globalEdges(), m_globalCenter()
{

}

Polygon::Polygon(std::initializer_list<sf::Vector2f> vertices) : m_vertices(), m_origin(), m_angle(0), m_globalVertices(), m_globalEdges(), m_globalCenter()
{
    m_vertices.resize(vertices.size());
    std::copy(vertices.begin(), vertices.end(), m_vertices.begin());
}

Polygon::Polygon(std::vector<sf::Vector2f> vertices) : m_vertices(vertices), m_origin(), m_angle(0), m_globalVertices(), m_globalEdges(), m_globalCenter()
{

}

Polygon::~Polygon()
{

}

void Polygon::DrawDebugPolygon(sf::RenderTarget &target)
{
    sf::VertexArray poly(sf::LinesStrip);

    for(unsigned int i = 0; i < m_globalVertices.size(); i++)
    {
        poly.append(sf::Vertex(m_globalVertices[i]));
    }

    poly.append(sf::Vertex(m_globalVertices[0]));

    target.draw(poly);
}

sf::Vector2f Polygon::RotatePoint(sf::Vector2f point, float angle)
{
    sf::Vector2f futurePoint;
    float radAngle = angle * M_PI / 180.f;

    futurePoint.x = point.x * cos(radAngle) - point.y * sin(radAngle);
    futurePoint.y = point.y * cos(radAngle) + point.x * sin(radAngle);

    return futurePoint;
}

Polygon Polygon::Rectangle(float width, float height)
{
    return Polygon({sf::Vector2f(-width/2, -height/2), sf::Vector2f(+width/2, -height/2), sf::Vector2f(+width/2, +height/2), sf::Vector2f(-width/2, +height/2)});
}

void Polygon::ComputeGlobalVertices()
{
    m_globalVertices.resize(m_vertices.size());

    for(unsigned int i = 0; i < m_vertices.size(); i++)
    {
        m_globalVertices[i] = Polygon::RotatePoint(m_vertices[i], m_angle);
        m_globalVertices[i] += m_origin;
    }
}

void Polygon::ComputeGlobalEdges()
{
    m_globalEdges.resize(m_globalVertices.size());

    for(unsigned int i = 0; i < m_globalVertices.size() - 1; i++)
    {
        m_globalEdges[i] = m_globalVertices[i+1] - m_globalVertices[i];
    }

    m_globalEdges[m_globalVertices.size() - 1] = m_globalVertices[0] - m_globalVertices[m_globalVertices.size() - 1];
}

void Polygon::ComputeGlobalCenter()
{

}

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

sf::Vector2f ProjectPolygon(sf::Vector2f &axis, Polygon &p)
{
    sf::Vector2f minMax;
    //First value for the min (x) and max (y)
    minMax.x = minMax.y = DotProduct(axis, p.GetGlobalVertices()[0]);

    for(unsigned int i = 0; i < p.GetGlobalVertices().size(); i++)
    {
        float dotPr = DotProduct(axis, p.GetGlobalVertices()[i]);
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

//Collision test
bool PolygonCollision(Polygon &p1, Polygon &p2)
{
    bool intersect = true;

    unsigned int p1EdgesCount = p1.GetGlobalEdges().size();
    unsigned int p2EdgesCount = p2.GetGlobalEdges().size();

    for(unsigned int i = 0; i < p1EdgesCount + p2EdgesCount; i++)
    {
        sf::Vector2f edge;

        if(i < p1EdgesCount)
        {
            edge = p1.GetGlobalEdges()[i];
        }
        else
        {
            edge = p2.GetGlobalEdges()[i - p1EdgesCount];
        }

        sf::Vector2f axis(-edge.y, edge.x);
        axis = Normalise(axis);

        sf::Vector2f p1Project(ProjectPolygon(axis, p1));
        sf::Vector2f p2Project(ProjectPolygon(axis, p2));

        if(IntervalDistance(p1Project, p2Project) > 0.f)
        {
            intersect = false;
        }
    }

    return intersect;
}

}
