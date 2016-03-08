#ifndef YAPG_GAME_STATE_EDITOR_ENTRYPROPERTYWIDGET_H
#define YAPG_GAME_STATE_EDITOR_ENTRYPROPERTYWIDGET_H

#include <boost/lexical_cast.hpp>

#include <SFGUI/Entry.hpp>

#include "Components/TemplateComponent.hpp"
#include "Editor/PropertyWidget.hpp"

namespace editor
{

template<typename T>
class EntryPropertyWidget : public PropertyWidget
{
public:
    EntryPropertyWidget(entityx::Entity entity, const lua::EntityTemplate::Parameter& parameter) :
        PropertyWidget(entity, parameter),
        m_entry(sfg::Entry::Create())
    {
        m_entry->GetSignal(sfg::Widget::OnLostFocus).Connect(std::bind(&EntryPropertyWidget<T>::setParameterFromEntry, this));

        updateEntryFromParameter();
    }

    virtual ~EntryPropertyWidget() {}

    virtual sfg::Widget::Ptr getWidget() { return m_entry; }

private:
    void updateEntryFromParameter()
    {
        auto templateComponent = m_entity.component<components::TemplateComponent>();
        m_entry->SetText(
            boost::lexical_cast<std::string>(
                boost::any_cast<T>(
                    templateComponent->parametersHelper.getParameter(m_parameter.name)
                )
            )
        );
    }

    void setParameterFromEntry()
    {
        auto templateComponent = m_entity.component<components::TemplateComponent>();
        std::string entryText = m_entry->GetText();

        templateComponent->parametersHelper.setParameter(
            m_parameter.name,
            boost::any(
                boost::lexical_cast<T>(entryText)
            )
        );
    }

    sfg::Entry::Ptr m_entry;
};

}

#endif
