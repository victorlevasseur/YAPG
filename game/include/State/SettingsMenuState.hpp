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

    simplgui::Button::Ptr m_leftButton;
    simplgui::Button::Ptr m_rightButton;
    simplgui::Button::Ptr m_jumpButton;

    simplgui::Button::Ptr m_returnButton;

    //Change keys mechanism
    int m_currentPlayer;
    simplgui::Button::Ptr m_keyToChangeButton;
};

}

#endif
