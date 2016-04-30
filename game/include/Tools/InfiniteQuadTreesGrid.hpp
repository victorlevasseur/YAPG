#ifndef YAPG_GAME_TOOLS_INFINITEQUADTREESGRID_H
#define YAPG_GAME_TOOLS_INFINITEQUADTREESGRID_H

#include <iostream>
#include <map>

#include "Tools/ObjectsWithIdCollection.hpp"
#include "Tools/QuadTree.hpp"

namespace tools
{

template<class Object, class AABBGetter = AABBGetter<Object>>
class InfiniteQuadTreesGrid
{
    using SubQuadTree = QuadTree<Object, AABBGetter>;
    using ObjectsCollection = ObjectsWithIdCollection<Object, AABBGetter>;

public:
    InfiniteQuadTreesGrid(float gridWidth, float gridHeight, std::size_t maxLeafs = 16, std::size_t maxRecursion = 8) :
        m_gridWidth(gridWidth),
        m_gridHeight(gridHeight),
        m_maxLeafs(maxLeafs),
        m_maxRecursion(maxRecursion),
        m_quadtrees(),
        m_objectsCollection(std::make_shared<ObjectsCollection>())
    {

    }

    /**
     * Insert an object into the quadtrees-grid
     */
    bool insert(const Object& value)
    {
        //Test if the object is already in the collection.
        if(m_objectsCollection->contains(value))
            return false;

        std::size_t newId = m_objectsCollection->newId();
        m_objectsCollection->get()[newId] = value;

        sf::Rect<std::ptrdiff_t> gridIndexes = getGridIndexesFromAABB(AABBGetter::getAABB(value));
        for(std::ptrdiff_t xIndex = gridIndexes.left; xIndex <= gridIndexes.left + gridIndexes.width; ++xIndex)
        {
            for(std::ptrdiff_t yIndex = gridIndexes.top; yIndex <= gridIndexes.top + gridIndexes.height; ++yIndex)
            {
                safeAt(xIndex, yIndex).insert(value);
            }
        }

        return true;
    }

    /**
     * Tells the quadtrees-grid that an object AABB (position and size) was updated.
     */
    void update(const Object& value)
    {
        //If not already in objects collection, insert it
        if(!m_objectsCollection->contains(value))
        {
            insert(value);
            return;
        }

        //Create the non-existing quadtrees if needed
        sf::Rect<std::ptrdiff_t> gridIndexes = getGridIndexesFromAABB(AABBGetter::getAABB(value));
        for(std::ptrdiff_t xIndex = gridIndexes.left; xIndex <= gridIndexes.left + gridIndexes.width; ++xIndex)
        {
            for(std::ptrdiff_t yIndex = gridIndexes.top; yIndex <= gridIndexes.top + gridIndexes.height; ++yIndex)
            {
                safeAt(xIndex, yIndex).update(value); //This will create the quadtrees if needed.
            }
        }

        auto previousQuadtrees = m_objectsCollection->getAssignments()[m_objectsCollection->getKeyOf(value)];
        //Make a copy. Avoids undefined behavior if the list of assigned quadtress changes during updates
        for(auto& quadtree : previousQuadtrees)
            quadtree->update(value);
    }

    void erase(const Object& value)
    {
        if(!m_objectsCollection->contains(value))
        {
            return;
        }

        sf::Rect<std::ptrdiff_t> gridIndexes = getGridIndexesFromAABB(AABBGetter::getAABB(value));
        for(std::ptrdiff_t xIndex = gridIndexes.left; xIndex <= gridIndexes.left + gridIndexes.width; ++xIndex)
        {
            for(std::ptrdiff_t yIndex = gridIndexes.top; yIndex <= gridIndexes.top + gridIndexes.height; ++yIndex)
            {
                safeAt(xIndex, yIndex).erase(value);
            }
        }

        m_objectsCollection->get().erase(m_objectsCollection->getKeyOf(value));
    }

    void printContent()
    {
        std::cout << "Content of the Grid:" << std::endl;
        std::cout << "====================" << std::endl;
        for(const auto& quadtree : m_quadtrees)
        {
            quadtree.second.printContent(2);
        }
    }

private:
    SubQuadTree& safeAt(std::ptrdiff_t xIndex, std::ptrdiff_t yIndex)
    {
        if(m_quadtrees.count(std::make_pair(xIndex, yIndex)) == 0)
        {
            m_quadtrees.emplace(
                std::make_pair(
                    std::make_pair(xIndex, yIndex),
                    SubQuadTree(
                        sf::FloatRect(static_cast<float>(xIndex) * m_gridWidth, static_cast<float>(yIndex) * m_gridHeight, m_gridWidth, m_gridHeight),
                        m_maxLeafs,
                        m_maxRecursion,
                        m_objectsCollection
                    )
                )
            );
        }

        return m_quadtrees.at(std::make_pair(xIndex, yIndex));
    }

    sf::Rect<std::ptrdiff_t> getGridIndexesFromAABB(sf::FloatRect AABB)
    {
        return sf::Rect<std::ptrdiff_t>(
            static_cast<std::ptrdiff_t>(AABB.left / m_gridWidth),
            static_cast<std::ptrdiff_t>(AABB.top / m_gridHeight),
            static_cast<std::ptrdiff_t>((AABB.left + AABB.width) / m_gridWidth) - static_cast<std::ptrdiff_t>(AABB.left / m_gridWidth),
            static_cast<std::ptrdiff_t>((AABB.top + AABB.height) / m_gridHeight) - static_cast<std::ptrdiff_t>(AABB.top / m_gridHeight)
        );
    }

    const float m_gridWidth;
    const float m_gridHeight;

    const std::size_t m_maxLeafs;
    const std::size_t m_maxRecursion;

    std::map<std::pair<std::ptrdiff_t, std::ptrdiff_t>, SubQuadTree> m_quadtrees;

    std::shared_ptr<ObjectsCollection> m_objectsCollection;
};

}

#endif
