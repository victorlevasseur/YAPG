#include <codecvt>
#include <iostream>
#include <locale>

#include <SFML/Graphics.hpp>

#include "simplgui/Button.h"
#include "simplgui/Event.h"
#include "simplgui/FileResourcesGetter.h"
#include "simplgui/TextBox.h"
#include "simplgui/Tools.h"

void whenTextChanged(simplgui::TextBox::Ptr textBox, const std::u32string &str)
{
    std::cout << simplgui::tools::getSfString(str).toAnsiString() << std::endl;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "A test of SimplGUI");

    //THEME
    simplgui::Theme myTheme = simplgui::Theme::defaultTheme();

    //RESOURCES GETTER
    auto resGetter = simplgui::FileResourcesGetter::create();

    //TEXTBOX
    simplgui::TextBox::Ptr textBox = simplgui::TextBox::create(resGetter);
    textBox->setPosition(sf::Vector2f(100, 100));
    textBox->setText(U"Some text");
    textBox->setSize(sf::Vector2f(300, simplgui::AUTO_SIZE));
    textBox->setTheme(myTheme);

    //BUTTON
    simplgui::Button::Ptr button = simplgui::Button::create(resGetter);
    button->setPosition(sf::Vector2f(408, 100));
    button->setMinSize(sf::Vector2f(48, simplgui::NO_MIN_SIZE));
    button->setTheme(myTheme);
    button->setLabel(U"Clear text");

    textBox->onTextChanged.bind(whenTextChanged);
    button->onClicked.bind([&](simplgui::Button::Ptr button)
    {
        textBox->setText(U"");
    });

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            textBox->processEvent(simplgui::Event(event, window));
            button->processEvent(simplgui::Event(event, window));
        }

        auto dt = clock.restart();

        textBox->update(dt);
        button->update(dt);

        window.clear(sf::Color(200, 200, 200));
        window.draw(*textBox);
        window.draw(*button);
        window.display();
    }

    return 0;
}
