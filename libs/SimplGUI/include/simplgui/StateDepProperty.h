#ifndef SIMPLGUI_STATEDEPPROPERTY_H
#define SIMPLGUI_STATEDEPPROPERTY_H

#include <initializer_list>

namespace simplgui
{

template<typename T>
class StateDepProperty
{
public:
    /**
     * Create a StateDepProperty object and initialize each state properties
     * with **all**.
     */
    StateDepProperty(const T &all) : normal(all), hovered(all), focused(all), focusedAndHovered(all), focusedAndClicked(all), disabled(all), disabledAndHovered(all) { }

    StateDepProperty& setAll(const T &all)
    {
        normal = all;
        hovered = all;
        focused = all;
        focusedAndHovered = all;
        focusedAndClicked = all;
        disabled = all;
        disabledAndHovered = all;
        return *this;
    }

    StateDepProperty& setHovered(const T &_hovered)
    {
        hovered = _hovered;
        return *this;
    }

    StateDepProperty& setFocused(const T &_focused)
    {
        focused = _focused;
        focusedAndHovered = _focused;
        focusedAndClicked = _focused;
        return *this;
    }

    StateDepProperty& setFocusedAndHovered(const T &_focusedAndHovered)
    {
        focusedAndHovered = _focusedAndHovered;
        return *this;
    }

    StateDepProperty& setFocusedAndClicked(const T &_focusedAndClicked)
    {
        focusedAndClicked = _focusedAndClicked;
        return *this;
    }

    StateDepProperty& setDisabled(const T &_disabled)
    {
        disabled = _disabled;
        disabledAndHovered = _disabled;
        return *this;
    }

    StateDepProperty& setDisabledAndHovered(const T &_disabledAndHovered)
    {
        disabledAndHovered = _disabledAndHovered;
        return *this;
    }

    T normal;
    T hovered;
    T focused;
    T focusedAndHovered;
    T focusedAndClicked;
    T disabled;
    T disabledAndHovered;
};

}

#endif
