#ifndef YAPG_GAME_STATE_EDITOR_PROPERTIESMANAGER_H
#define YAPG_GAME_STATE_EDITOR_PROPERTIESMANAGER_H

#include <map>
#include <typeindex>
#include <vector>

#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Table.hpp>

#include "entityx/entityx.h"

#include "Editor/PropertyWidget.hpp"

namespace editor
{

class PropertiesManager
{
public:
    PropertiesManager(sfg::ScrolledWindow::Ptr propertiesScrolled);

    void setCurrentEntity(entityx::Entity currentEntity);

    template<typename T, typename U>
    void registerPropertyWidget();

private:
    sfg::ScrolledWindow::Ptr m_propertiesScrolled;
    sfg::Table::Ptr m_propertiesTable;
    std::vector<std::unique_ptr<PropertyWidget>> m_propertiesWidgets;

    entityx::Entity m_currentEntity;

    std::map<std::type_index, std::function<std::unique_ptr<PropertyWidget>(entityx::Entity, const lua::EntityTemplate::Parameter&)>> m_registeredWidgets;
};

}

#include "Editor/PropertiesManager.inl"

#endif
