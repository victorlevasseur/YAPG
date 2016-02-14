#ifndef SIMPLGUI_THEME_H
#define SIMPLGUI_THEME_H

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>

#include "simplgui/Any.h"
#include "simplgui/StateDepProperty.h"

namespace simplgui
{

using StateColor = StateDepProperty<sf::Color>;

class Theme
{
public:
    Theme() : m_properties() {};

    bool hasProperty(const std::string &name) const
    {
        return m_properties.count(name) != 0;
    }

    template<class T>
    bool hasPropertyOfType(const std::string &name) const
    {
        return hasProperty(name) && m_properties.at(name).is<T>();
    }

    template<class T>
    const T& getProperty(const std::string &name) const
    {
        return m_properties.at(name).as<T>();
    }

    template<class T>
    const T& getProperty(const std::string &name, const T& defaultValue) const
    {
        if(!hasPropertyOfType<T>(name))
            return defaultValue;
        else
            return getProperty<T>(name);
    }

    template<class T>
    void setProperty(const std::string &name, const T &value)
    {
        m_properties[name] = Any(value);
    }

    template<class T>
    void setProperty(const std::string &name, T &&value)
    {
        m_properties[name] = Any(std::forward<T>(value));
    }

    static Theme defaultTheme()
    {
        Theme th;

        th.setProperty<std::string>("font", "LiberationSans.ttf");
        th.setProperty<unsigned int>("font_size", 30);

        th.setProperty<StateColor>("background_color", StateColor(sf::Color(255, 255, 255, 180)).setFocused(sf::Color(255, 255, 255, 255)));
        th.setProperty<StateColor>("border_color", StateColor(sf::Color(0, 0, 0, 255)).setFocused(sf::Color(128, 128, 255, 255)));
        th.setProperty<float>("border_thickness", 2.f);

        th.setProperty<StateColor>("button_background_color", StateColor(sf::Color(180, 180, 180, 255)).setFocusedAndClicked(sf::Color(200, 200, 200, 255)));
        th.setProperty<StateColor>("button_border_color", StateColor(sf::Color(0, 0, 0, 255)).setFocusedAndClicked(sf::Color(128, 128, 128, 255)));
        th.setProperty<float>("button_border_thickness", 2.f);

        th.setProperty<StateColor>("selection_color", StateColor(sf::Color(100, 100, 255, 180)).setFocused(sf::Color(100, 100, 255, 255)));
        th.setProperty<StateColor>("selection_border_color", StateColor(sf::Color(128, 128, 255, 180)).setFocused(sf::Color(128, 128, 255, 255)));
        th.setProperty<float>("selection_border_thickness", 1.f);

        th.setProperty<StateColor>("text_color", StateColor(sf::Color(0, 0, 0, 255)));

        return th;
    }

private:
    std::map<std::string, Any> m_properties;
};

}

#endif
