#ifndef YAPG_GAME_STATE_EDITOR_PROPERTYWIDGET_H
#define YAPG_GAME_STATE_EDITOR_PROPERTYWIDGET_H

#include "entityx/entityx.h"

#include "Template/EntityTemplate.hpp"

namespace yapg
{

class PropertyWidget
{
public:
    PropertyWidget(entityx::Entity entity, const EntityTemplate::Parameter& parameter);
    virtual ~PropertyWidget();

    virtual void display() = 0;

protected:
    entityx::Entity m_entity;
    const EntityTemplate::Parameter& m_parameter;
};

}

#endif
