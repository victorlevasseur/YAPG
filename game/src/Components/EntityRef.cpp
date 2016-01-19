#include "Components/EntityRef.hpp"

namespace game
{
namespace components
{

EntityRef::EntityRef(int serialId) :
    m_serialId(serialId),
    m_entity()
{

}

void EntityRef::setSerialId(int serialId)
{
    m_serialId = serialId;
    m_entity = entityx::Entity();
}

void EntityRef::setEntity(entityx::Entity entity)
{
    m_entity = entity;
}

}
}
