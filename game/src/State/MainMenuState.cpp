#include "State/MainMenuState.hpp"

#include <cmath>

#include "simplgui/Theme.h"
#include "simplgui/Tools.h"

#include "State/LevelState.hpp"
#include "State/StateEngine.hpp"

namespace state
{

MainMenuState::MainMenuState(StateEngine& stateEngine, resources::TexturesManager& texturesManager, resources::FontManager& fontManager, resources::SoundManager& soundManager, settings::SettingsManager& settingsManager) :
    State(stateEngine),
    m_texturesManager(texturesManager),
    m_fontManager(fontManager),
    m_soundManager(soundManager),
    m_settingsManager(settingsManager),
    m_guiResGetter(resources::GuiResourcesGetter::create(m_fontManager)),
    m_logoTexture(texturesManager.requestResource("menu/YAPGLogo.png")),
    m_logoSprite(*m_logoTexture),
    m_levelPathTextBox(simplgui::TextBox::create(m_guiResGetter)),
    m_playLevelButton(simplgui::Button::create(m_guiResGetter)),
    m_settingsButton(simplgui::Button::create(m_guiResGetter)),
    m_quitButton(simplgui::Button::create(m_guiResGetter)),
    m_playerAnimations(),
    m_playerSprite(texturesManager.requestResource("menu/spritesheet_players.png"), m_playerAnimations),
    m_draggingPlayer(false),
    m_offsetToPlayer(0.f, 0.f),
    m_groundTexture(texturesManager.requestResource("menu/ground.png")),
    m_groundSprite(*m_groundTexture),
    m_backgroundSoundBuffer(soundManager.requestResource("menu/bensound-clearday.ogg")),
    m_backgroundSound(*m_backgroundSoundBuffer)
{
    //Theme
    simplgui::Theme theme = simplgui::Theme::defaultTheme();
    theme.setProperty<float>("border_thickness", 1.f);
    theme.setProperty<float>("button_border_thickness", 1.f);

    m_levelPathTextBox->setTheme(theme);
    m_playLevelButton->setTheme(theme);
    m_settingsButton->setTheme(theme);
    m_quitButton->setTheme(theme);

    //Logo
    m_logoSprite.setOrigin(m_logoSprite.getLocalBounds().width/2.f, m_logoSprite.getLocalBounds().height/2.f);
    m_logoSprite.setPosition(sf::Vector2f(512.f, 120.f));

    //Level path textbox
    m_levelPathTextBox->setPosition(sf::Vector2f(100.f, 250.f));
    m_levelPathTextBox->setSize(sf::Vector2f(624.f, simplgui::AUTO_SIZE));
    //Default level
    m_levelPathTextBox->setText(U"level.lua");

    //Level play button
    m_playLevelButton->setPosition(sf::Vector2f(734.f, 250.f));
    m_playLevelButton->setSize(sf::Vector2f(190.f, simplgui::AUTO_SIZE));
    m_playLevelButton->setLabel(U"Play it !");
    m_playLevelButton->onClicked.bind([&](simplgui::Button::Ptr widget)
    {
        getStateEngine().stopAndStartState
        <state::LevelState, std::string, resources::TexturesManager&, settings::SettingsManager&>(
            simplgui::tools::getSfString(m_levelPathTextBox->getText()).toAnsiString(), m_texturesManager, m_settingsManager
        );
    });

    //Settings button
    m_settingsButton->setPosition(sf::Vector2f(100.f, 650.f));
    m_settingsButton->setSize(sf::Vector2f(200.f, simplgui::AUTO_SIZE));
    m_settingsButton->setLabel(U"Settings");

    //Quit button
    m_quitButton->setPosition(sf::Vector2f(724.f, 650.f));
    m_quitButton->setSize(sf::Vector2f(200.f, simplgui::AUTO_SIZE));
    m_quitButton->setLabel(U"Quit");

    //Main menu animations
    // - The player
    m_playerAnimations.emplace("default", animation::Animation(
        std::vector<animation::Frame>
        {
            animation::Frame{sf::IntRect(512, 582, 128, 186), 1.f},
            animation::Frame{sf::IntRect(512, 326, 128, 186), 1.f}
        }
    ));
    m_playerAnimations["default"].setDuration(0.2f);
    m_playerAnimations.emplace("jump", animation::Animation(
        std::vector<animation::Frame>
        {
            animation::Frame{sf::IntRect(512, 1606, 128, 186), 1.f},
        }
    ));
    m_playerAnimations["jump"].setDuration(1.f);

    m_playerSprite.setCurrentAnimation("default");

    m_playerSprite.setPosition(sf::Vector2f(420.f, 370.f));
    m_playerSprite.setScale(sf::Vector2f(128.f, 186.f));
    // - The ground
    m_groundTexture->setRepeated(true);
    m_groundSprite.setPosition(sf::Vector2f(0.f, 556.f));
    m_groundSprite.setTextureRect(sf::IntRect(0, 0, 1024.f + 128.f, 384.f));

    //Sound init
    m_backgroundSound.setLoop(true);
    m_backgroundSound.setVolume(20.f);
}

void MainMenuState::onStop()
{
    m_backgroundSound.stop();
}

void MainMenuState::onPause()
{

}

void MainMenuState::onUnpause()
{

}

void MainMenuState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    simplgui::Event guiEvent(event, target);

    m_levelPathTextBox->processEvent(guiEvent);
    m_playLevelButton->processEvent(guiEvent);
    m_settingsButton->processEvent(guiEvent);
    m_quitButton->processEvent(guiEvent);

    if(event.type == sf::Event::MouseButtonPressed)
    {
        if(!m_draggingPlayer)
        {
            sf::Vector2f realCoords = target.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
            );

            sf::FloatRect playerRect(
                m_playerSprite.getPosition().x,
                m_playerSprite.getPosition().y,
                m_playerSprite.getScale().x,
                m_playerSprite.getScale().y
            );

            if(playerRect.contains(realCoords))
            {
                m_draggingPlayer = true;
                m_offsetToPlayer = sf::Vector2f(
                    realCoords.x - playerRect.left,
                    realCoords.y - playerRect.top
                );
                m_playerSprite.setCurrentAnimation("jump");
            }
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        m_draggingPlayer = false;
    }
}

void MainMenuState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(140, 200, 255));

    target.draw(m_logoSprite);

    target.draw(m_groundSprite);

    target.draw(*m_levelPathTextBox);
    target.draw(*m_playLevelButton);
    target.draw(*m_settingsButton);
    target.draw(*m_quitButton);

    target.draw(m_playerSprite);

    //TODO: Move it to doUpdate, but need a ref to the RenderTarget!
    //Move the player to the mouse if it is dragged
    if(m_draggingPlayer)
    {
        //TODO: Forbid the user to put the player sprite under the ground
        sf::Vector2f playerDestination =
            target.mapPixelToCoords(sf::Mouse::getPosition(dynamic_cast<sf::RenderWindow&>(target))) - m_offsetToPlayer;
        m_playerSprite.setPosition(
            playerDestination.x, std::min(playerDestination.y, 370.f)
        );
    }
}

void MainMenuState::doStart()
{
    m_backgroundSound.play();
}

void MainMenuState::doUpdate(sf::Time dt)
{
    float scale = 1.f + 0.05f * std::sin(getTimeSinceStart().asSeconds());
    m_logoSprite.setScale(sf::Vector2f(scale, scale));

    m_levelPathTextBox->update(dt);
    m_playLevelButton->update(dt);
    m_settingsButton->update(dt);
    m_quitButton->update(dt);

    m_playerSprite.update(dt.asSeconds());

    //Move the ground
    m_groundSprite.setPosition(sf::Vector2f(
        m_groundSprite.getPosition().x - dt.asSeconds() * 200,
        m_groundSprite.getPosition().y
    ));
    while(m_groundSprite.getPosition().x <= -128.f)
    {
        m_groundSprite.setPosition(sf::Vector2f(
            m_groundSprite.getPosition().x + 128.f,
            m_groundSprite.getPosition().y
        ));
    }

    //Move the player sprite back to its position after a drag
    if(!m_draggingPlayer)
    {
        //Drag down the player when not dragged by the mouse
        if(m_playerSprite.getPosition().y < 370.f)
        {
            m_playerSprite.setPosition(
                m_playerSprite.getPosition().x, m_playerSprite.getPosition().y + 400 * dt.asSeconds()
            );
        }
        else if(m_playerSprite.getPosition().y > 370.f)
        {
            m_playerSprite.setPosition(
                sf::Vector2f(m_playerSprite.getPosition().x, 370.f)
            );
        }

        //Reset the animation when he's on the floor
        if(m_playerSprite.getPosition().y >= 370.f && m_playerSprite.getCurrentAnimation() != "default")
        {
            m_playerSprite.setCurrentAnimation("default");
        }
    }
}

}
