#include "State/SettingsMenuState.hpp"

#include "Settings/KeySettings.hpp"
#include "State/StateEngine.hpp"
#include "Tools/KeyStrings.hpp"
#include "Tools/StringTools.hpp"

namespace state
{

SettingsMenuState::SettingsMenuState(StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager) :
    State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_guiResGetter(resources::GuiResourcesGetter::create(m_resourcesManager.getFonts())),
    m_menuFont(m_resourcesManager.getFonts().requestResource("LiberationSans.ttf")),
    m_settingsTitle("Settings", *m_menuFont, 60u),
    m_keySettingsTitle("Keyboard settings", *m_menuFont, 40u),
    m_leftButtonLabel("Left:", *m_menuFont),
    m_leftButton(simplgui::Button::create(m_guiResGetter)),
    m_rightButtonLabel("Right:", *m_menuFont),
    m_rightButton(simplgui::Button::create(m_guiResGetter)),
    m_jumpButtonLabel("Jump:", *m_menuFont),
    m_jumpButton(simplgui::Button::create(m_guiResGetter)),
    m_returnButton(simplgui::Button::create(m_guiResGetter)),
    m_currentPlayer(0),
    m_keyToChangeButton(nullptr)
{
    //Theme
    simplgui::Theme theme = simplgui::Theme::defaultTheme();
    theme.setProperty<float>("border_thickness", 1.f);
    theme.setProperty<float>("button_border_thickness", 1.f);

    m_leftButton->setTheme(theme);
    m_rightButton->setTheme(theme);
    m_jumpButton->setTheme(theme);
    m_returnButton->setTheme(theme);

    m_settingsTitle.setPosition(sf::Vector2f(512.f - m_settingsTitle.getGlobalBounds().width/2.f, 50.f));
    m_settingsTitle.setColor(sf::Color::Black);
    m_keySettingsTitle.setPosition(sf::Vector2f(100.f, 180.f));
    m_keySettingsTitle.setColor(sf::Color::Black);
    //Keys buttons
    m_leftButtonLabel.setPosition(sf::Vector2f(100.f, 250.f));
    m_leftButtonLabel.setColor(sf::Color::Black);
    m_leftButton->setPosition(sf::Vector2f(200.f, 250.f));
    m_leftButton->setSize(sf::Vector2f(150.f, simplgui::AUTO_SIZE));
    m_rightButtonLabel.setPosition(sf::Vector2f(100.f, 300.f));
    m_rightButtonLabel.setColor(sf::Color::Black);
    m_rightButton->setPosition(sf::Vector2f(200.f, 300.f));
    m_rightButton->setSize(sf::Vector2f(150.f, simplgui::AUTO_SIZE));
    m_jumpButtonLabel.setPosition(sf::Vector2f(100.f, 350.f));
    m_jumpButtonLabel.setColor(sf::Color::Black);
    m_jumpButton->setPosition(sf::Vector2f(200.f, 350.f));
    m_jumpButton->setSize(sf::Vector2f(150.f, simplgui::AUTO_SIZE));

    auto keyChangeCallback = [&](simplgui::Button::Ptr button)
    {
        if(m_keyToChangeButton) //Disable the previous button if there's one
        {
            updateKeysButtonsFromSettings(m_currentPlayer);
            m_keyToChangeButton = nullptr;
        }

        //Set the current button to receive the key
        m_keyToChangeButton = button;
        button->setLabel(U"Type to set");
    };
    m_leftButton->onClicked.bind(keyChangeCallback);
    m_rightButton->onClicked.bind(keyChangeCallback);
    m_jumpButton->onClicked.bind(keyChangeCallback);

    //Return button
    m_returnButton->setPosition(sf::Vector2f(412.f, 650.f));
    m_returnButton->setSize(sf::Vector2f(200.f, simplgui::AUTO_SIZE));
    m_returnButton->setLabel(U"Back");
    m_returnButton->onClicked.bind([&](simplgui::Button::Ptr button) {
        getStateEngine().stopStateAndUnpause();
    });

    //Init the buttons with their corresponding keys
    updateKeysButtonsFromSettings(0);
}

void SettingsMenuState::onStop()
{

}

void SettingsMenuState::onPause()
{

}

void SettingsMenuState::onUnpause()
{

}

void SettingsMenuState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    m_leftButton->processEvent(simplgui::Event(event, target));
    m_rightButton->processEvent(simplgui::Event(event, target));
    m_jumpButton->processEvent(simplgui::Event(event, target));

    m_returnButton->processEvent(simplgui::Event(event, target));

    if(m_keyToChangeButton && event.type == sf::Event::KeyPressed)
    {
        //Change the key
        if(event.key.code != sf::Keyboard::Escape)
            m_keyToChangeButton->setLabel(tools::getUTF32String(tools::keyToString(event.key.code)));
        else
            updateKeysButtonsFromSettings(m_currentPlayer);

        //Disable the key change mechanism
        m_keyToChangeButton = nullptr;
        updateSettingsFromKeysButtons();
    }
}

void SettingsMenuState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(140, 200, 255));

    target.draw(m_settingsTitle);
    target.draw(m_keySettingsTitle);
    target.draw(m_leftButtonLabel);
    target.draw(*m_leftButton);
    target.draw(m_rightButtonLabel);
    target.draw(*m_rightButton);
    target.draw(m_jumpButtonLabel);
    target.draw(*m_jumpButton);

    target.draw(*m_returnButton);
}

void SettingsMenuState::doStart()
{

}

void SettingsMenuState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    m_leftButton->update(dt);
    m_rightButton->update(dt);
    m_jumpButton->update(dt);

    m_returnButton->update(dt);
}

void SettingsMenuState::updateKeysButtonsFromSettings(int playerNumber)
{
    settings::KeySettings::PlayerKeys& playerKeys = m_settingsManager.getKeySettings().getPlayerKeys(playerNumber);

    m_leftButton->setLabel(tools::getUTF32String(tools::keyToString(playerKeys.leftKey)));
    m_rightButton->setLabel(tools::getUTF32String(tools::keyToString(playerKeys.rightKey)));
    m_jumpButton->setLabel(tools::getUTF32String(tools::keyToString(playerKeys.jumpKey)));

    m_currentPlayer = playerNumber;
}

void SettingsMenuState::updateSettingsFromKeysButtons()
{
    settings::KeySettings::PlayerKeys& playerKeys = m_settingsManager.getKeySettings().getPlayerKeys(m_currentPlayer);

    playerKeys.leftKey = tools::stringToKey(tools::getLocalString(m_leftButton->getLabel()));
    playerKeys.rightKey = tools::stringToKey(tools::getLocalString(m_rightButton->getLabel()));
    playerKeys.jumpKey = tools::stringToKey(tools::getLocalString(m_jumpButton->getLabel()));
}

}
