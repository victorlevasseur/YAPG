#ifndef SIMPLGUI_BUTTON_H
#define SIMPLGUI_BUTTON_H

#include <SFML/Graphics.hpp>
#include "simplgui/Widget.h"
#include "simplgui/Connector.h"

namespace simplgui
{

class Button : public Widget
{
public:
    using Ptr = std::shared_ptr<Button>;
    using ConstPtr = std::shared_ptr<const Button>;

    static std::shared_ptr<Button> create(std::shared_ptr<ResourcesGetter> resGetter = nullptr);
    virtual ~Button() {};

    std::u32string getLabel() const;
    void setLabel(const std::u32string& str);

    Connector<Button> onClicked;

protected:
    Button(std::shared_ptr<ResourcesGetter> resGetter = nullptr);

    virtual void doProcessEvent(simplgui::Event event);
    virtual void doUpdate(sf::Time dt);

    virtual void onSizeUpdated();

    virtual sf::Vector2f doCalculateAutoSize() const;

    virtual void doThemeUpdate();

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    std::u32string m_label;

    std::shared_ptr<sf::Font> m_font;
};

}

#endif
