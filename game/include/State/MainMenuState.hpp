#ifndef YAPG_GAME_STATE_MAINMENUSTATE_H
#define YAPG_GAME_STATE_MAINMENUSTATE_H

#include <memory>
#include <vector>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <SFGUI/Button.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Window.hpp>

#include "Animation/AnimatedSprite.hpp"
#include "Animation/Animation.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace state
{

class MainMenuState : public State
{
public:
    MainMenuState(StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop);

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    struct PlayerKeysWidgets
    {
        sfg::Button::Ptr leftKeyButton;
        sfg::Button::Ptr rightKeyButton;
        sfg::Button::Ptr jumpKeyButton;
    };

    void updateKeysButtonsFromSettings();
    void updateSettingsFromKeysButtons();

    resources::AllResourcesManagers& m_resourcesManager;
    settings::SettingsManager& m_settingsManager;

    //Menu elements
    std::shared_ptr<sf::Texture> m_logoTexture;
    sf::Sprite m_logoSprite;

    //GUI
    sfg::SFGUI& m_sfgui;
    sfg::Desktop& m_desktop;
    sfg::Window::Ptr m_mainMenuWindow;
    sfg::Window::Ptr m_settingsWindow;
    std::vector<PlayerKeysWidgets> m_playersKeysWidgets;
    sfg::Button::Ptr m_lastSelectedKeyButton;

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
