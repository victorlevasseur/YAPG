#include "simplgui/Button.h"

#include <iostream>
#include "simplgui/Renderer.h"

namespace simplgui
{

std::shared_ptr<Button> Button::create(std::shared_ptr<ResourcesGetter> resGetter)
{
    return std::shared_ptr<Button>(new Button(resGetter));
}

Button::Button(std::shared_ptr<ResourcesGetter> resGetter) :
    Widget(resGetter),
    onClicked(),
    m_label(),
    m_font()
{

}

std::u32string Button::getLabel() const
{
    return m_label;
}

void Button::setLabel(const std::u32string& str)
{
    m_label = str;
    needAutoSizeUpdate();
}

void Button::doProcessEvent(simplgui::Event event)
{
    if(event.type = simplgui::Event::MouseButtonReleased)
    {
        sf::Transform globalTr = getGlobalTransform();
        sf::FloatRect widgetRect(sf::Vector2f(0.f, 0.f), getEffectiveSize());

        widgetRect = globalTr.transformRect(widgetRect);

        if(widgetRect.contains(event.mouseButton.x, event.mouseButton.y))
        {
            onClicked.call(shared_from_this());
        }
    }
}

void Button::doUpdate(sf::Time dt)
{

}

void Button::onSizeUpdated()
{

}

sf::Vector2f Button::doCalculateAutoSize() const
{
    sf::Vector2f currentTextSize = Renderer::getTextSize(m_label, *m_font, getTheme().getProperty<unsigned int>("font_size", 30));
    //Calculate the height using a custom text (so as the height is always correct even if the text is empty)
    sf::Vector2f textSize = Renderer::getTextSize(U"abfgjl", *m_font, getTheme().getProperty<unsigned int>("font_size", 30));

    return sf::Vector2f(currentTextSize.x + 6.f, textSize.y + 6.f);
}

void Button::doThemeUpdate()
{
    if(getResourcesGetter().expired())
    {
        std::cout << "[WARNING] simplgui::TextBox: No resources getter affected, can't load the font." << std::endl;
        return;
    }
    std::shared_ptr<ResourcesGetter> resGetter(getResourcesGetter());

    m_font = resGetter->loadFont(getTheme().getProperty<std::string>("font", "Liberation.ttf"));

    needAutoSizeUpdate();
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    Renderer::drawButtonRectangle(target, shared_from_this(), sf::FloatRect({0.f, 0.f}, getEffectiveSize()));
    Renderer::drawText(target, shared_from_this(), m_label, *m_font, {3.f, 3.f});
}

}
