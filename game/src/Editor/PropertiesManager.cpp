#include "Editor/PropertiesManager.hpp"

#include <boost/lexical_cast.hpp>

#include "Components/TemplateComponent.hpp"
#include "Editor/EntryPropertyWidget.hpp"

namespace editor
{

PropertiesManager::PropertiesManager() :
    m_registeredWidgets(),
    m_currentEntity(),
    m_propertiesNames(),
    m_propertiesWidgets()
{

}

void PropertiesManager::setCurrentEntity(entityx::Entity currentEntity)
{
    m_currentEntity = currentEntity;

    //Clear the properties widgets
    m_propertiesNames.clear();
    m_propertiesWidgets.clear();

    if(m_currentEntity)
    {
        auto templateComponent = m_currentEntity.component<components::TemplateComponent>();

        //Add the new properties widgets
        for(auto it = templateComponent->parametersHelper.parametersBegin();
            it != templateComponent->parametersHelper.parametersEnd();
            ++it)
        {
            //Add the corresponding property widget (if not available, put a text saying that the parameter is not editable from the editor !)
            std::type_index parameterType = templateComponent->parametersHelper.getParameterType(it->first);
            if(m_registeredWidgets.count(parameterType) > 0)
            {
                m_propertiesNames.push_back(it->second.friendlyName);
                m_propertiesWidgets.emplace_back(
                    m_registeredWidgets.at(parameterType)(m_currentEntity, it->second)
                );
            }
        }
    }
}

void PropertiesManager::display()
{
    for(std::size_t i = 0; i < m_propertiesNames.size(); ++i)
    {
        ImGui::PushID(i);
        ImGui::Text(m_propertiesNames[i].data());
        ImGui::SameLine();
        m_propertiesWidgets[i]->display();
        ImGui::PopID();
    }
}

}
