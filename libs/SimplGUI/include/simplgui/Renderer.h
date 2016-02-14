#ifndef SIMPLGUI_RENDERER_H
#define SIMPLGUI_RENDERER_H

#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace sf
{
class Font;
}

namespace simplgui
{

class Widget;

/**
 * Class providing tools to draw the most common shapes for widgets.
 */
class Renderer
{
public:
    /**
     * Draw a background rectangle.
     */
    static void drawBackgroundRectangle(sf::RenderTarget &target, std::shared_ptr<const Widget> widget, sf::FloatRect rectangle);

    /**
     * Draw a button rectangle.
     */
    static void drawButtonRectangle(sf::RenderTarget &target, std::shared_ptr<const Widget> widget, sf::FloatRect rectangle);

    /**
     * Draw a selection rectangle.
     */
    static void drawSelectionRectangle(sf::RenderTarget &target, std::shared_ptr<const Widget> widget, sf::FloatRect rectangle);

    /**
     * Draw a rectangle with custom colors, position and outline thickness.
     */
    static void drawRectangle(
        sf::RenderTarget &target,
        sf::FloatRect rectangle,
        float outline,
        sf::Color fillColor,
        sf::Color outlineColor,
        sf::Transform transform = sf::Transform()
        );

    /**
     * Get the size of a text.
     */
    static sf::Vector2f getTextSize(const std::u32string &str, const sf::Font &font, unsigned int size);

    /**
     * Get the position of a character in a text
     */
    static sf::Vector2f getCharPosInText(const std::u32string &str, const sf::Font &font, unsigned int size, int charIndex);

    /**
     * Draw a text according to the theme properties of a widget.
     */
    static void drawText(
        sf::RenderTarget &target,
        std::shared_ptr<const Widget> widget,
        const std::u32string &str,
        const sf::Font &font,
        sf::Vector2f position
        );

    /**
     * Draw a text with specified color.
     */
    static void drawText(
        sf::RenderTarget &target,
        const std::u32string &str,
        const sf::Font &font,
        unsigned int size,
        sf::Vector2f position,
        sf::Color color,
        sf::Transform transform = sf::Transform()
        );

private:
};

}

#endif
