#ifndef YAPG_GAME_STATE_MAINMENUSTATE_H
#define YAPG_GAME_STATE_MAINMENUSTATE_H

#include <memory>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "simplgui/Button.h"
#include "simplgui/TextBox.h"

#include "Animation/AnimatedSprite.hpp"
#include "Animation/Animation.hpp"
#include "Resources/GuiResourcesGetter.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace state
{

class MainMenuState : public State
{
public:
    MainMenuState(StateEngine& stateEngine, resources::TexturesManager& texturesManager, resources::FontManager& fontManager, resources::SoundManager& soundManager, settings::SettingsManager& settingsManager);

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doUpdate(sf::Time dt);

private:
    resources::TexturesManager& m_texturesManager;
    resources::FontManager& m_fontManager;
    resources::SoundManager& m_soundManager;
    settings::SettingsManager& m_settingsManager;

    resources::GuiResourcesGetter::Ptr m_guiResGetter;

    //Menu elements
    std::shared_ptr<sf::Texture> m_logoTexture;
    sf::Sprite m_logoSprite;

    simplgui::TextBox::Ptr m_levelPathTextBox;
    simplgui::Button::Ptr m_playLevelButton;

    simplgui::Button::Ptr m_settingsButton;
    simplgui::Button::Ptr m_quitButton;

    //Menu anim
    // - Player
    std::map<std::string, animation::Animation> m_playerAnimations;
    animation::AnimatedSprite m_playerSprite;
    bool m_draggingPlayer;
    sf::Vector2f m_offsetToPlayer;

    // - Ground
    std::shared_ptr<sf::Texture> m_groundTexture;
    sf::Sprite m_groundSprite;

    //Menu music
    std::shared_ptr<sf::SoundBuffer> m_backgroundSoundBuffer;
    sf::Sound m_backgroundSound;
};

}

#endif
