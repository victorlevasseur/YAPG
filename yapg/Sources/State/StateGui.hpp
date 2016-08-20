#ifndef YAPG_GAME_STATE_STATEGUI_H
#define YAPG_GAME_STATE_STATEGUI_H

#include <set>

#include <TGUI/TGUI.hpp>

namespace yapg
{

class StateGui
{
public:
    StateGui(tgui::Gui& gui, tgui::Theme::Ptr theme);

	tgui::WidgetConverter createWidget(const std::string& name);

    template<class WidgetType>
	std::shared_ptr<WidgetType> add(const std::shared_ptr<WidgetType>& widget, const sf::String& name = sf::String())
    {
        m_stateWidgetsContainer->add(widget, name);
        return widget;
    }

	bool remove(const std::shared_ptr<tgui::Widget>& widget);

    tgui::Widget::Ptr get(const sf::String& name, bool recursive = false);

    template<class WidgetType>
    std::shared_ptr<WidgetType> get(const sf::String& name, bool recursive = false)
    {
        return std::dynamic_pointer_cast<WidgetType>(get(name, recursive));
    }

    void onStart();
    void onStop();
    void onPause();
    void onUnpause();

private:
    tgui::Gui& m_gui;
	tgui::Theme::Ptr m_theme;

    tgui::Panel::Ptr m_stateWidgetsContainer;
};

}

#endif
