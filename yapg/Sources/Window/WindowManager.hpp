#ifndef YAPG_GAME_WINDOW_WINDOWMANAGER_H
#define YAPG_GAME_WINDOW_WINDOWMANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/String.hpp>

#include <TGUI/TGUI.hpp>

#include "Tools/Observable.hpp"
#include "Tools/Observer.hpp"

namespace yapg { class StateEngine; }

namespace yapg
{

class WindowManager : public Observer
{
public:
    WindowManager(StateEngine& engine, const sf::String& windowTitle);
    virtual ~WindowManager();

    void run();

    virtual void update(Observable& observable, void* value = nullptr);

    const tgui::Gui& getGui() const { return m_gui; }
    tgui::Gui& getGui() { return m_gui; }

    tgui::Theme::Ptr getGuiTheme() { return m_guiTheme; }

private:
    sf::RenderWindow m_window;
    StateEngine& m_engine;

    tgui::Gui m_gui;
    tgui::Theme::Ptr m_guiTheme;
};

}

#endif
