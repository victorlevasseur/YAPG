#include "simplgui/Event.h"

#include <iostream>

namespace simplgui
{

Event::Event(sf::Event event, const sf::RenderTarget &target)
{
    init(event, target, target.getView());
}

Event::Event(sf::Event event, const sf::RenderTarget &target, sf::View view)
{
    init(event, target, view);
}

void Event::init(sf::Event event, const sf::RenderTarget &target, sf::View view)
{
    if(event.type == sf::Event::MouseButtonPressed ||
       event.type == sf::Event::MouseButtonReleased)
    {
        type = (event.type == sf::Event::MouseButtonPressed) ?
                    MouseButtonPressed : MouseButtonReleased;
        mouseButton.button = event.mouseButton.button;
        sf::Vector2f pos(target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), view));
        mouseButton.x = pos.x;
        mouseButton.y = pos.y;
    }
    else if(event.type == sf::Event::MouseMoved)
    {
        type = MouseMoved;
        sf::Vector2f pos(target.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), view));
        mouseMove.x = pos.x;
        mouseMove.y = pos.y;
    }
    else if(event.type == sf::Event::TextEntered)
    {
        type = TextEntered;
        text.unicode = event.text.unicode;
    }
    else if(event.type == sf::Event::KeyPressed ||
            event.type == sf::Event::KeyReleased)
    {
        type = (event.type == sf::Event::KeyPressed) ?
                    KeyPressed : KeyReleased;
        key.code = event.key.code;
        key.alt = event.key.alt;
        key.control = event.key.control;
        key.shift = event.key.shift;
        key.system = event.key.system;
    }
    else
    {
        type = Unsupported;
        std::cout << "[NOTE] simplgui::Event: Unsupported event created" << std::endl;
    }
}

}
