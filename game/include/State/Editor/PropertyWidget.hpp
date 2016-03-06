#ifndef YAPG_GAME_STATE_EDITOR_PROPERTYWIDGET_H
#define YAPG_GAME_STATE_EDITOR_PROPERTYWIDGET_H

#include "entityx/entityx.h"

#include <SFGUI/Widget.hpp>

#include "Lua/EntityTemplate.hpp"

namespace state
{
namespace editor
{

class PropertyWidget
{
public:
    PropertyWidget(entityx::Entity entity, const lua::EntityTemplate::Parameter& parameter);
    virtual ~PropertyWidget();

    virtual sfg::Widget::Ptr getWidget() = 0;

protected:
    entityx::Entity m_entity;
    const lua::EntityTemplate::Parameter& m_parameter;
};

}
}

#endif
