#include "State/StateGui.hpp"

namespace yapg
{

StateGui::StateGui(tgui::Gui& gui, tgui::Theme::Ptr theme) :
    m_gui(gui),
    m_theme(theme),
    m_stateWidgetsContainer(std::make_shared<tgui::Panel>(1024, 768))
{
    m_stateWidgetsContainer->setBackgroundColor(tgui::Color(255, 255, 255, 0));
}

tgui::WidgetConverter StateGui::createWidget(const std::string& name)
{
	return m_theme->load(name);
}

tgui::Widget::Ptr StateGui::get(const sf::String& name, bool recursive)
{
    return m_stateWidgetsContainer->get(name, recursive);
}

bool StateGui::remove(const std::shared_ptr<tgui::Widget>& widget)
{
	return m_stateWidgetsContainer->remove(widget);
}

void StateGui::onStart()
{
    m_gui.add(m_stateWidgetsContainer);
}

void StateGui::onStop()
{
    m_gui.remove(m_stateWidgetsContainer);
}

void StateGui::onPause()
{
    m_stateWidgetsContainer->hide();
}

void StateGui::onUnpause()
{
    m_stateWidgetsContainer->show();
}

}
