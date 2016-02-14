#include "simplgui/Widget.h"

namespace simplgui
{

Widget::Widget(std::shared_ptr<ResourcesGetter> resGetter) :
    sf::Drawable(),
    sf::Transformable(),
    m_parent(),
    m_size(AUTO_SIZE, AUTO_SIZE),
    m_minSize(NO_MIN_SIZE, NO_MIN_SIZE),
    m_maxSize(NO_MAX_SIZE, NO_MAX_SIZE),
    m_focus(false),
    m_click(false),
    m_theme(Theme::defaultTheme()),
    m_resGetter(resGetter),
    m_tmp_autoSize(),
    m_tmp_autoSizeNeedUpdate(false)
{

}

void Widget::processEvent(simplgui::Event event)
{
    sf::Transform globalTr = getGlobalTransform();
    sf::FloatRect widgetRect(sf::Vector2f(0.f, 0.f), getEffectiveSize());

    widgetRect = globalTr.transformRect(widgetRect);

    if(event.type == simplgui::Event::MouseButtonPressed)
    {
        if(widgetRect.contains(event.mouseButton.x, event.mouseButton.y))
        {
            setFocused(true);
            m_click = true;
        }
        else //TODO: Allow widgets to go outside their effective size rectangle (like combobox with their unfoldable list)
        {
            setFocused(false);
        }
    }
    else if(event.type == simplgui::Event::MouseButtonReleased)
    {
        m_click = false;
    }
    else if(event.type == simplgui::Event::MouseMoved)
    {
        if(widgetRect.contains(event.mouseMove.x, event.mouseMove.y))
        {
            
        }
        else //TODO: Allow widgets to go outside their effective size rectangle (like combobox with their unfoldable list)
        {
            m_click = false;
        }
    }

    doProcessEvent(event);
}

void Widget::update(sf::Time dt)
{
    doUpdate(dt);
}

sf::Vector2f Widget::getSize() const
{
    return m_size;
}

void Widget::setSize(sf::Vector2f size)
{
    m_size = size;
    onSizeUpdated();
}

sf::Vector2f Widget::getEffectiveSize() const
{
    sf::Vector2f autoSize = doCalculateAutoSize();

    return sf::Vector2f(
        std::min(std::max(m_size.x == AUTO_SIZE ? autoSize.x : m_size.x, m_minSize.x), m_maxSize.x),
        std::min(std::max(m_size.y == AUTO_SIZE ? autoSize.y : m_size.y, m_minSize.y), m_maxSize.y)
        );
}

sf::Vector2f Widget::getMinSize() const
{
    return m_minSize;
}

void Widget::setMinSize(sf::Vector2f minSize)
{
    m_minSize = minSize;
    onSizeUpdated();
}

sf::Vector2f Widget::getMaxSize() const
{
    return m_maxSize;
}

void Widget::setMaxSize(sf::Vector2f maxSize)
{
    m_maxSize = maxSize;
    onSizeUpdated();
}

sf::Transform Widget::getGlobalTransform() const
{
    return (m_parent.expired() ? getTransform() : (m_parent.lock()->getGlobalTransform() * getTransform()));
}

sf::Vector2f Widget::calculateAutoSize() const
{
    if(m_tmp_autoSizeNeedUpdate)
    {
        m_tmp_autoSize = doCalculateAutoSize();
        m_tmp_autoSizeNeedUpdate = false;
    }

    return m_tmp_autoSize;
}

void Widget::needAutoSizeUpdate() const
{
    m_tmp_autoSizeNeedUpdate = true;
}


}
