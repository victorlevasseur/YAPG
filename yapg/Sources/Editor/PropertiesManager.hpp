#ifndef YAPG_GAME_STATE_EDITOR_PROPERTIESMANAGER_H
#define YAPG_GAME_STATE_EDITOR_PROPERTIESMANAGER_H

#include <map>
#include <memory>
#include <typeindex>
#include <vector>

#include "entityx/entityx.h"

#include "Editor/PropertyWidget.hpp"

namespace yapg
{

class PropertiesManager
{
public:
    PropertiesManager();

    void setCurrentEntity(entityx::Entity currentEntity);

    void display();

    template<typename T, typename U>
    void registerPropertyWidget();

private:
    std::map<std::type_index, std::function<std::unique_ptr<PropertyWidget>(entityx::Entity, const EntityTemplate::Parameter&)>> m_registeredWidgets;

    entityx::Entity m_currentEntity;

    std::vector<std::string> m_propertiesNames;
    std::vector<std::unique_ptr<PropertyWidget>> m_propertiesWidgets;
};

}

#include "Editor/PropertiesManager.inl"

#endif
