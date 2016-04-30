#ifndef YAPG_GAME_TOOLS_OBJECTSWITHIDCOLLECTION_H
#define YAPG_GAME_TOOLS_OBJECTSWITHIDCOLLECTION_H

#include <algorithm>
#include <map>

#include "Tools/AABBGetter.hpp"

namespace tools{ template<class Object, class AABBGetter> class QuadTree; }

namespace tools
{

template<class Object, class AABBGetter = AABBGetter<Object>>
class ObjectsWithIdCollection
{
    using ObjectId = std::size_t;
    using ObjectsMap = std::map<ObjectId, Object>;
    using ObjectsAssignments = std::map<ObjectId, std::set<QuadTree<Object, AABBGetter>*>>;

public:
    ObjectsWithIdCollection()
        : m_objects(), m_nextId(0)
    {

    }

    const ObjectsMap& get() const { return m_objects; }
    ObjectsMap& get() { return m_objects; }

    bool contains(const Object& object) const
    {
        return std::find_if(m_objects.cbegin(), m_objects.cend(), [&object](const std::pair<ObjectId, Object>& pair) { return pair.second == object; }) != m_objects.cend();
    }

    ObjectId getKeyOf(const Object& object) const
    {
        return std::find_if(m_objects.cbegin(), m_objects.cend(), [&object](const std::pair<ObjectId, Object>& pair) { return pair.second == object; })->first;
    }

    std::size_t newId() const {return m_nextId++; }

    const ObjectsAssignments& getAssignments() const { return m_assignments; }
    ObjectsAssignments& getAssignments() { return m_assignments; }

private:
    ObjectsMap m_objects;
    ObjectsAssignments m_assignments;
    mutable std::size_t m_nextId;
};

}

#endif
