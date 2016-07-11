#include "Level/SerializedEntityGetter.hpp"


namespace yapg
{

SerializedEntityGetter::SerializedEntityGetter() :
    m_unserializedEntities()
{

}

void SerializedEntityGetter::registerEntity(entityx::Entity entity, int id)
{
    m_unserializedEntities[id] = entity;
}

entityx::Entity SerializedEntityGetter::getEntityFromSerializationId(int id) const
{
    return m_unserializedEntities.at(id);
}

int SerializedEntityGetter::getSerializationIdFromEntity(entityx::Entity entity) const
{
    auto it = m_unserializedEntities.find(entity);
    if(it == m_unserializedEntities.end())
    {
        return -1;
    }
    else
    {
        return it->first;
    }
}

}
