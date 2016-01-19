#include "Level/SerializedEntityGetter.hpp"

namespace game
{
namespace level
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

}
}
