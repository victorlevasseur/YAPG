#ifndef YAPG_GAME_STATE_MAINMENUSTATE_H
#define YAPG_GAME_STATE_MAINMENUSTATE_H

#include <memory>
#include <vector>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

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
    MainMenuState(StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doStop();

    virtual void doPause();

    virtual void doUnpause();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    void updateKeysButtonsFromSettings();
    void updateSettingsFromKeysButtons();

    resources::AllResourcesManagers& m_resourcesManager;
    settings::SettingsManager& m_settingsManager;

    //Menu elements
    std::shared_ptr<sf::Texture> m_logoTexture;
    sf::Sprite m_logoSprite;

    // - Settings window
    std::array<std::array<char[32], 3>, 4> m_playersKeys;
    char* m_selectedKeyButton;

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

    //Menu logos
    std::vector<std::shared_ptr<sf::Texture>> m_logosTextures;
    sf::Sprite m_boostLogo;
    sf::Sprite m_entityxLogo;
    sf::Sprite m_sfmlLogo;
    sf::Sprite m_solLogo;
};

}

#endif
