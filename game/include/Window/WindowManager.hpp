#ifndef YAPG_GAME_WINDOW_WINDOWMANAGER_H
#define YAPG_GAME_WINDOW_WINDOWMANAGER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/String.hpp>

#include "Tools/Observable.hpp"
#include "Tools/Observer.hpp"

namespace game { namespace state { class StateEngine; } }

namespace game
{
namespace window
{

class WindowManager : public tools::Observer
{
public:
    WindowManager(state::StateEngine& engine, const sf::String& windowTitle);

    void run();

    virtual void update(tools::Observable& observable, void* value = nullptr);

private:
    sf::RenderWindow m_window;
    state::StateEngine& m_engine;
};

}
}

#endif
