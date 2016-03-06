#ifndef YAPG_GAME_STATE_EDITOR_PROPERTIESMANAGER_H
#define YAPG_GAME_STATE_EDITOR_PROPERTIESMANAGER_H

#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>

#include "entityx/entityx.h"

#include "State/Editor/PropertyWidget.hpp"

namespace state
{
namespace editor
{

class PropertiesManager
{
public:
    PropertiesManager(sfg::ScrolledWindow::Ptr propertiesScrolled);

    void setCurrentEntity(entityx::Entity currentEntity);

    void updateParametersValues();

private:
    sfg::ScrolledWindow::Ptr m_propertiesScrolled;
    sfg::Table::Ptr m_propertiesTable;
    std::vector<std::unique_ptr<PropertyWidget>> m_propertiesWidgets;

    entityx::Entity m_currentEntity;
};

}
}

#endif
