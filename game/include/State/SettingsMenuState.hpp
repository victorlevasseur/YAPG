#ifndef YAPG_GAME_STATE_SETTINGSMENUSTATE_H
#define YAPG_GAME_STATE_SETTINGSMENUSTATE_H

#include <memory>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "simplgui/Button.h"
#include "simplgui/TextBox.h"

#include "Resources/GuiResourcesGetter.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace state
{

class SettingsMenuState : public State
{
public:
    SettingsMenuState(StateEngine& stateEngine, resources::TexturesManager& texturesManager, resources::FontManager& fontManager, resources::SoundManager& soundManager, settings::SettingsManager& settingsManager);

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    void updateKeysButtonsFromSettings(int playerNumber);
    void updateSettingsFromKeysButtons();

    resources::TexturesManager& m_texturesManager;
    resources::FontManager& m_fontManager;
    resources::SoundManager& m_soundManager;
    settings::SettingsManager& m_settingsManager;

    resources::GuiResourcesGetter::Ptr m_guiResGetter;
    std::shared_ptr<sf::Font> m_menuFont;

    sf::Text m_settingsTitle;
    sf::Text m_keySettingsTitle;
    sf::Text m_leftButtonLabel;
    simplgui::Button::Ptr m_leftButton;
    sf::Text m_rightButtonLabel;
    simplgui::Button::Ptr m_rightButton;
    sf::Text m_jumpButtonLabel;
    simplgui::Button::Ptr m_jumpButton;

    simplgui::Button::Ptr m_returnButton;

    //Change keys mechanism
    int m_currentPlayer;
    simplgui::Button::Ptr m_keyToChangeButton;
};

}

#endif
