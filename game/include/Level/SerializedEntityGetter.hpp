#ifndef YAPG_GAME_LEVEL_ENTITYLOADER_H
#define YAPG_GAME_LEVEL_ENTITYLOADER_H

#include <map>

#include "entityx/entityx.h"


namespace level
{

/**
 * \brief stores a map between serialized entities id and Entity (handles).
 *
 * When an entity store the id of another one in a serialized lua level file,
 * the link needs to be established when the entities are instanciated in the
 * game. This class allow an entity's component to get a Entity (handle) from
 * the id the entity had in the lua level file.
 */
class SerializedEntityGetter
{
public:
    SerializedEntityGetter();

    void registerEntity(entityx::Entity entity, int id);

    entityx::Entity getEntityFromSerializationId(int id) const;

private:
    std::map<int, entityx::Entity> m_unserializedEntities;
};

}

#endif
