#ifndef YAPG_GAME_STATE_EDITOR_ENTRYPROPERTYWIDGET_H
#define YAPG_GAME_STATE_EDITOR_ENTRYPROPERTYWIDGET_H

#include <boost/lexical_cast.hpp>

#include "Gui/imgui.h"

#include "Editor/PropertyWidget.hpp"
#include "Lua/Template/TemplateComponent.hpp"
#include "NativeGui/TextBoxWindow.hpp"

namespace yapg
{

template<typename T, bool MULTILINETEXT = false>
class EntryPropertyWidget : public PropertyWidget
{
public:
    EntryPropertyWidget(entityx::Entity entity, const EntityTemplate::Parameter& parameter) :
        PropertyWidget(entity, parameter),
        m_wasActiveBefore(false)
    {
        updateEntryFromParameter();
    }

    virtual ~EntryPropertyWidget()
    {

    }

    virtual void display()
    {
        ImGui::InputText("", m_propertyContent, 1024);
        if(ImGui::IsItemActive())
        {
            m_wasActiveBefore = true;
        }
        else
        {
            if(m_wasActiveBefore)
            {
                setParameterFromEntry();
            }
            else
            {
                updateEntryFromParameter();
            }
            m_wasActiveBefore = false;
        }
    }

private:
    void updateEntryFromParameter()
    {
        auto templateComponent = m_entity.component<TemplateComponent>();
        std::string propertyAsString =
            boost::lexical_cast<std::string>(
                boost::any_cast<T>(
                    templateComponent->parametersHelper.getParameter(m_parameter.name)
                )
            );

        strncpy(m_propertyContent, propertyAsString.data(), 1023);
        m_propertyContent[1023] = '\0';
    }

    void setParameterFromEntry()
    {
        auto templateComponent = m_entity.component<TemplateComponent>();

        templateComponent->parametersHelper.setParameter(
            m_parameter.name,
            boost::any(
                boost::lexical_cast<T>(std::string(m_propertyContent))
            )
        );
    }

    char m_propertyContent[1024];
    bool m_wasActiveBefore;
};

}

#endif
