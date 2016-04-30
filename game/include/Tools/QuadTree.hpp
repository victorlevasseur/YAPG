#ifndef YAPG_GAME_TOOLS_QUADTREE_H
#define YAPG_GAME_TOOLS_QUADTREE_H

#include <map>
#include <memory>
#include <set>

#include <SFML/Graphics/Rect.hpp>

#include "Tools/ObjectsWithIdCollection.hpp"

namespace tools
{

template<class Object, class AABBGetter = AABBGetter<Object>>
class QuadTree
{
    using ObjectId = std::size_t;
    using ThisQuadTree = QuadTree<Object, AABBGetter>;
    using ObjectsCollection = ObjectsWithIdCollection<Object, AABBGetter>;

public:
    /**
     * Create a QuadTree (it can share the same objects collection than another one, useful to create a sub quadtree).
     */
    QuadTree(sf::FloatRect area, std::size_t maxLeafs = 16, std::size_t maxRecursion = 8, std::shared_ptr<ObjectsCollection> objectsCollection = std::shared_ptr<ObjectsCollection>())
        :
        m_area(area),
        m_maxLeafs(maxLeafs),
        m_maxRecursion(maxRecursion),
        m_objectsCollection(objectsCollection),
        m_northWest(nullptr),
        m_northEast(nullptr),
        m_southWest(nullptr),
        m_southEast(nullptr),
        m_leafs()
    {

    }

    /**
     * Insert an object into the quadtree
     */
    bool insert(const Object& value)
    {
        if(AABBGetter::getAABB(value).intersects(m_area))
        {
            //It is inside this quadtree area.
            if(m_leafs.size() >= m_maxLeafs)
                subdivide();

            if(m_northWest && m_northEast && m_southWest && m_southEast)
            {
                m_northWest->insert(value);
                m_northEast->insert(value);
                m_southWest->insert(value);
                m_southEast->insert(value);
            }
            else
            {
                //Insert the object into the leafs
                ObjectId key = m_objectsCollection->getKeyOf(value);
                m_leafs.insert(key);
                m_objectsCollection->getAssignments()[key].insert(this);
            }

            return true;
        }

        return false; //Not inside the QuadTree area, return false
    }

    /**
     * Tells the quadtree that an object AABB (position and size) was updated.
     */
    void update(const Object& value)
    {
        //If not already in objects collection, stop
        if(!m_objectsCollection->contains(value))
        {
            return;
        }

        if(!m_northWest)
        {
            //This is a leaf quadtree, test if we can insert the value in it
            std::size_t objectId = m_objectsCollection->getKeyOf(value);

            if(AABBGetter::getAABB(value).intersects(m_area))
            {
                if(m_leafs.size() >= m_maxLeafs && m_maxRecursion > 0)
                    subdivide(); //This will subdivide the quadtree and update will be called on the subquadtrees
                else
                {
                    m_objectsCollection->getAssignments()[objectId].insert(this);
                    m_leafs.insert(objectId);
                }
            }
            else
            {
                m_objectsCollection->getAssignments()[objectId].erase(this);
                m_leafs.erase(objectId);
            }
        }

        if(m_northWest)
        {
            //This quadtree owns sub-quadtrees, call update on them
            m_northWest->update(value);
            m_northEast->update(value);
            m_southWest->update(value);
            m_southEast->update(value);
        }
    }

    void erase(const Object& value)
    {
        if(!AABBGetter::getAABB(value).intersects(m_area))
            return; //It's not in this quadtree for sure.

        if(!m_northWest)
        {
            //Remove the value from the leafs.
            std::size_t valueId = m_objectsCollection->getKeyOf(value);
            m_objectsCollection->getAssignments()[valueId].erase(this);
            m_leafs.erase(valueId);
        }
        else
        {
            m_northWest->erase(value);
            m_northEast->erase(value);
            m_southWest->erase(value);
            m_southEast->erase(value);
        }
    }

    void getObjectsIntersectingAABB(sf::FloatRect AABB, std::set<Object>& result) const
    {
        if(!AABB.intersects(m_area))
            return;

        if(!m_northEast)
        {
            for(auto objectId : m_leafs)
            {
                if(m_objectsCollection->get().count(objectId) == 0)
                    continue;

                Object object = m_objectsCollection->get()[objectId];
                if(AABB.intersects(AABBGetter::getAABB(object)))
                    result.insert(object);
            }
        }
        else
        {
            m_northWest->getObjectsIntersectingAABB(AABB, result);
            m_northEast->getObjectsIntersectingAABB(AABB, result);
            m_southWest->getObjectsIntersectingAABB(AABB, result);
            m_southEast->getObjectsIntersectingAABB(AABB, result);
        }
    }

    void printContent(int spaces) const
    {
        std::cout << std::string(spaces, ' ') << "Quadtree ("
            << m_area.left << ";" << m_area.top << ") -> "
            << m_area.left + m_area.width << ";" << m_area.top + m_area.height << ":" << std::endl;

        if(!m_northWest)
        {
            std::cout << std::string(spaces, ' ') << "|Content : ";
            for(const auto& leaf : m_leafs)
                std::cout << leaf << ", ";
            std::cout << "." << std::endl;
        }
        else
        {
            m_northWest->printContent(spaces + 2);
            m_northEast->printContent(spaces + 2);
            m_southWest->printContent(spaces + 2);
            m_southEast->printContent(spaces + 2);
        }
    }

private:
    void subdivide()
    {
        if(m_northWest || m_northEast || m_southWest || m_southEast)
            return; //TODO: Exception (logic ?) ?

        if(m_maxRecursion == 0)
            return; //Do not divide if m_maxRecursion == 0

        //Instanciated the new sub QuadTrees
        m_northWest.reset(new QuadTree(sf::FloatRect(m_area.left, m_area.top, m_area.width/2.f, m_area.height/2.f), m_maxLeafs, m_maxRecursion-1, m_objectsCollection));
        m_northEast.reset(new QuadTree(sf::FloatRect(m_area.left + m_area.width/2.f, m_area.top, m_area.width/2.f, m_area.height/2.f), m_maxLeafs, m_maxRecursion-1, m_objectsCollection));
        m_southWest.reset(new QuadTree(sf::FloatRect(m_area.left, m_area.top + m_area.height/2.f, m_area.width/2.f, m_area.height/2.f), m_maxLeafs, m_maxRecursion-1, m_objectsCollection));
        m_southEast.reset(new QuadTree(sf::FloatRect(m_area.left + m_area.width/2.f, m_area.top + m_area.height/2.f, m_area.width/2.f, m_area.height/2.f), m_maxLeafs, m_maxRecursion-1, m_objectsCollection));

        //Move the leafs into the different
        for(const ObjectId& objectId : m_leafs)
        {
            const Object& object = m_objectsCollection->get().at(objectId);

            m_objectsCollection->getAssignments()[objectId].erase(this);

            m_northWest->insert(object);
            m_northEast->insert(object);
            m_southWest->insert(object);
            m_southEast->insert(object);
        }

        m_leafs.clear();
    }

    const sf::FloatRect m_area;
    const std::size_t m_maxLeafs;
    const std::size_t m_maxRecursion;

    std::shared_ptr<ObjectsCollection> m_objectsCollection;

    std::unique_ptr<ThisQuadTree> m_northWest;
    std::unique_ptr<ThisQuadTree> m_northEast;
    std::unique_ptr<ThisQuadTree> m_southWest;
    std::unique_ptr<ThisQuadTree> m_southEast;

    std::set<ObjectId> m_leafs;
};

}

#endif
