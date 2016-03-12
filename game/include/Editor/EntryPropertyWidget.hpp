#ifndef YAPG_GAME_STATE_EDITOR_ENTRYPROPERTYWIDGET_H
#define YAPG_GAME_STATE_EDITOR_ENTRYPROPERTYWIDGET_H

#include <boost/lexical_cast.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>

#include "Components/TemplateComponent.hpp"
#include "Editor/PropertyWidget.hpp"
#include "NativeGui/TextBoxWindow.hpp"

namespace editor
{

template<typename T, bool MULTILINETEXT = false>
class EntryPropertyWidget : public PropertyWidget
{
public:
    EntryPropertyWidget(entityx::Entity entity, const lua::EntityTemplate::Parameter& parameter) :
        PropertyWidget(entity, parameter),
        m_box(sfg::Box::Create()),
        m_entry(sfg::Entry::Create()),
        m_moreButton(sfg::Button::Create("..."))
    {
        m_box->PackEnd(m_entry, true, true);
        if(MULTILINETEXT)
        {
            m_moreButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&EntryPropertyWidget<T, MULTILINETEXT>::openMultilineTextWindow, this));
            m_box->PackEnd(m_moreButton, true, false);
        }

        m_entry->GetSignal(sfg::Widget::OnLostFocus).Connect(std::bind(&EntryPropertyWidget<T, MULTILINETEXT>::setParameterFromEntry, this));
        m_entry->SetRequisition(sf::Vector2f(130.f, 0.f));

        updateEntryFromParameter();
    }

    virtual ~EntryPropertyWidget() {}

    virtual sfg::Widget::Ptr getWidget() { return m_box; }

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

    void openMultilineTextWindow()
    {
        nativegui::TextBoxWindow textBoxWindow(m_entry->GetText(), "Enter text here...");
        textBoxWindow.run();

        m_entry->SetText(textBoxWindow.getText());
    }

    sfg::Box::Ptr m_box;
    sfg::Entry::Ptr m_entry;
    sfg::Button::Ptr m_moreButton;
};

}

#endif
