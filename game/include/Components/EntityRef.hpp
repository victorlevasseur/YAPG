#ifndef YAPG_GAME_COMPONENTS_ENTITYREF_H
#define YAPG_GAME_COMPONENTS_ENTITYREF_H

#include "entityx/entityx.h"

namespace game
{
namespace components
{

class EntityRef
{
public:
    explicit EntityRef(int serialId);

    int getSerialId() const { return m_serialId; }
    void setSerialId(int serialId);

    bool isUsable() const { return m_entity; };
    entityx::Entity getEntity() const { return m_entity; }
    void setEntity(entityx::Entity entity);

private:
    int m_serialId;
    entityx::Entity m_entity;
};

}
}

#endif
