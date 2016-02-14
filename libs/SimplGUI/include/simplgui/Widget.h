#ifndef SIMPLGUI_WIDGET_H
#define SIMPLGUI_WIDGET_H

#include <limits>
#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "simplgui/Event.h"
#include "simplgui/Theme.h"
#include "simplgui/ResourcesGetter.h"

namespace simplgui
{

constexpr float AUTO_SIZE = -1.f; ///< Use this constant with Widget::setSize to let the widget autosize itself. \relates Widget
constexpr float NO_MIN_SIZE = 0.f; ///< Use this constant with Widget::setMinSize to tell the widget that it doesn't have a minimum size. \relates Widget
constexpr float NO_MAX_SIZE = std::numeric_limits<float>::max(); ///< Use this constant with Widget::setMaxSize to tell the widget that it doesn't have a maximum size. \relates Widget

/**
 * \brief The base class for all widgets.
 *
 * It provides the base features for all widgets : focus management, parenting, positioning and size management.
 * **All widgets** are not copyable and have a protected constructor as they must be constructed using their respective
 * static **create** method (returning a shared pointer).
 *
 * This class inherit from sf::Transformable which provides methods to move, rotate and scale the widget. However, the
 * size is managed by this class itself.
 */
class Widget : public sf::Drawable, public sf::Transformable, public std::enable_shared_from_this<Widget>
{
public:
    using Ptr = std::shared_ptr<Widget>; ///< Shared pointer to Widget
    using ConstPtr = std::shared_ptr<const Widget>; ///< Shared pointer to const Widget

    Widget(const Widget &) = delete;
    Widget& operator=(const Widget &) = delete;

    /**
     * Widget's destructor
     */
    virtual ~Widget() {};

    /**
     * Process an event.
     * \param event the SFML event to process
     */
    void processEvent(simplgui::Event event);

    /**
     * Update the widget
     * \param dt the timestep since the last frame
     */
    void update(sf::Time dt);

    /**
     * \return the size of the widget (as defined by the user).
     * \note if the widget is auto sized (\ref setSize) this will not return the effective size (\ref getEffectiveSize).
     */
    sf::Vector2f getSize() const;

    /**
     * Define the size of the widget. Use simplgui::AUTO_SIZE to let the widget calculate its own size (and you can then define
     * its min and max size).
     */
    void setSize(sf::Vector2f size);

    /**
     * \return the real effective size calculated by the widget itself between its minimum and maximum size if autosize is activated or
     * the size forced by the user.
     */
    sf::Vector2f getEffectiveSize() const;

    /**
     * \return the minimum size of the widget.
     */
    sf::Vector2f getMinSize() const;

    /**
     * Define the minimum size of the widget.
     */
    void setMinSize(sf::Vector2f minSize);

    /**
     * \return the maximum size of the widget.
     */
    sf::Vector2f getMaxSize() const;

    /**
     * Define the maximum size of the widget.
     */
    void setMaxSize(sf::Vector2f maxSize);

    /**
     * Get the global transform (the global position, rotation and scale) of this widget according to its relative transform
     * and its parent widget, if it has one.
     */
    sf::Transform getGlobalTransform() const;

    /**
     * \return true if the widget is focused.
     */
    bool isFocused() const { return m_focus; }

    /**
     * Enable or not the focus of the widget.
     * \note If you manage widgets independently (without a common container), you will have to disable the focus on other widgets
     * manually.
     */
    void setFocused(bool focus) { m_focus = focus; }

    /**
     * \return true if the widget is clicked.
     */
    bool isClicked() const { return m_click; }

    const Theme& getTheme() const { return m_theme; }

    void setTheme(const Theme &theme) { m_theme = theme; doThemeUpdate(); }

    void setResourcesGetter(std::shared_ptr<ResourcesGetter> resGetter) { m_resGetter = resGetter; doThemeUpdate(); }

protected:
    /**
     * Private constructor initializing the widget without a resources getter.
     */
    Widget(std::shared_ptr<ResourcesGetter> resGetter = nullptr);

    /**
     * All widgets must overload this method to be able to process SFML events.
     */
    virtual void doProcessEvent(simplgui::Event event) = 0;

    /**
     * All widgets must overload this method to be able to update themselves each frame.
     */
    virtual void doUpdate(sf::Time dt) = 0;

    /**
     * Called everytime the widget's size change. Widgets can overload this method to react to the size change.
     */
    virtual void onSizeUpdated() {};

    /**
     * Calculate the prefered autosize of the widget (if needed, Widgets must call needAutoSizeUpdate() if they need
     * to update their autosize so as this method will refresh its autosize cache).
     * \return the prefered size
     */
    sf::Vector2f calculateAutoSize() const;

    /**
     * All widgets must overload this method to provide their prefered size.
     */
    virtual sf::Vector2f doCalculateAutoSize() const = 0;

    /**
     * Widgets can overload this method to react to a theme change (to update their visual appearance).
     */
    virtual void doThemeUpdate() {};

    /**
     * Call this method to force an update of the autosize (which is not calculated each time calculateAutoSize() is
     * called).
     */
    void needAutoSizeUpdate() const;

    /**
     * The inherited widgets must overload this method so as they can be drawn using :
     * \code
     * myRenderWindow.draw(*myWidget);
     * \endcode
     */
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;

    /**
     * \return the resources getter used by this widget.
     */
    std::weak_ptr<ResourcesGetter> getResourcesGetter() const { return m_resGetter; }

private:
    std::weak_ptr<Widget> m_parent;
    sf::Vector2f m_size;
    sf::Vector2f m_minSize;
    sf::Vector2f m_maxSize;

    bool m_focus;
    bool m_click;

    Theme m_theme;
    std::weak_ptr<ResourcesGetter> m_resGetter;

    mutable sf::Vector2f m_tmp_autoSize;
    mutable bool m_tmp_autoSizeNeedUpdate;
};

}

#endif
