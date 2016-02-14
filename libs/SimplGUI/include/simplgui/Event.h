#ifndef SIMPLGUI_EVENT_H
#define SIMPLGUI_EVENT_H

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

namespace simplgui
{

class Event
{
public:
    enum EventType
    {
        Unsupported,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        TextEntered,
        KeyPressed,
        KeyReleased
    };

    Event(sf::Event event, const sf::RenderTarget &target);
    Event(sf::Event event, const sf::RenderTarget &target, sf::View view);

    EventType type;

    union
    {
        struct
        {
            sf::Mouse::Button button; ///< Code of the button that has been pressed
            float x;      ///< X position of the mouse pointer, in view coordinates
            float y;
        } mouseButton;

        struct
        {
            float x;      ///< X position of the mouse pointer, in view coordinates
            float y;
        } mouseMove;

        sf::Event::TextEvent text;

        sf::Event::KeyEvent key;
    };

private:
    void init(sf::Event event, const sf::RenderTarget &target, sf::View view);
};

}

#endif
