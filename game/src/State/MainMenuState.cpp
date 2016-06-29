#include "State/MainMenuState.hpp"

#include <array>
#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>

#include "imgui.h"

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Table.hpp>

#include "Editor/LevelEditorState.hpp"
#include "Level/LevelState.hpp"
#include "Settings/KeySettings.hpp"
#include "State/StateEngine.hpp"
#include "Tools/KeyStrings.hpp"

namespace state
{

MainMenuState::MainMenuState(StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_logoTexture(m_resourcesManager.getTextures().requestResource("menu/YAPGLogo.png")),
    m_logoSprite(*m_logoTexture),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_selectedKeyButton(nullptr),
    m_playerAnimations(),
    m_playerSprite(m_resourcesManager.getTextures().requestResource("menu/spritesheet_players.png"), m_playerAnimations),
    m_draggingPlayer(false),
    m_offsetToPlayer(0.f, 0.f),
    m_groundTexture(m_resourcesManager.getTextures().requestResource("menu/ground.png")),
    m_groundSprite(*m_groundTexture),
    m_backgroundSoundBuffer(m_resourcesManager.getSounds().requestResource("menu/bensound-clearday.ogg")),
    m_backgroundSound(*m_backgroundSoundBuffer),
    m_logosTextures{
        m_resourcesManager.getTextures().requestResource("credits/boost.png"),
        m_resourcesManager.getTextures().requestResource("credits/entityx.png"),
        m_resourcesManager.getTextures().requestResource("credits/sfgui.png"),
        m_resourcesManager.getTextures().requestResource("credits/sfml.png"),
        m_resourcesManager.getTextures().requestResource("credits/sol.png")
    },
    m_boostLogo(*m_logosTextures[0]),
    m_entityxLogo(*m_logosTextures[1]),
    m_sfguiLogo(*m_logosTextures[2]),
    m_sfmlLogo(*m_logosTextures[3]),
    m_solLogo(*m_logosTextures[4])
{
    //Logo
    m_logoSprite.setOrigin(m_logoSprite.getLocalBounds().width/2.f, m_logoSprite.getLocalBounds().height/2.f);
    m_logoSprite.setPosition(sf::Vector2f(512.f, 120.f));

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

    //Libs logos
    m_boostLogo.setPosition(sf::Vector2f(90.f, 650.f));
    m_entityxLogo.setPosition(sf::Vector2f(260.f, 650.f));
    m_sfguiLogo.setPosition(sf::Vector2f(420.f, 650.f));
    m_sfmlLogo.setPosition(sf::Vector2f(590.f, 650.f));
    m_solLogo.setPosition(sf::Vector2f(760.f, 600.f));

    updateKeysButtonsFromSettings();
}

void MainMenuState::processEvent(sf::Event event, sf::RenderTarget &target)
{
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
    else if(event.type == sf::Event::KeyPressed)
    {
        if(m_selectedKeyButton != nullptr)
        {
            if(event.key.code != sf::Keyboard::Escape)
            {
                strncpy(m_selectedKeyButton, tools::keyToString(event.key.code).data(), 31);
                m_selectedKeyButton[31] = '\0';
            }

            updateSettingsFromKeysButtons();
            updateKeysButtonsFromSettings();
        }
    }
}

void MainMenuState::render(sf::RenderTarget& target)
{
    //ImGui interface
    ImGui::SetNextWindowPosCenter();
    ImGui::SetNextWindowSize(ImVec2(350, 200));
    ImGui::Begin("YAPG - Main menu");
        //Level selection
        static char levelName[512] = "level.xml";
        ImGui::InputText("", levelName, 512);
        ImGui::SameLine();
        if(ImGui::Button("Play !"))
        {
            getStateEngine().pauseAndStartState
                <level::LevelState, std::string, resources::AllResourcesManagers&, settings::SettingsManager&, sfg::SFGUI&, sfg::Desktop&>(
                std::string(levelName), m_resourcesManager, m_settingsManager, m_sfgui, m_desktop
            );
        }

        //Level editor
        if(ImGui::Button("Level editor"))
        {
            getStateEngine().pauseAndStartState
                <editor::LevelEditorState, resources::AllResourcesManagers&, settings::SettingsManager&, sfg::SFGUI&, sfg::Desktop&>(
                m_resourcesManager, m_settingsManager, m_sfgui, m_desktop
            );
        }

        //Settings
        if(ImGui::Button("Settings..."))
        {
            ImGui::OpenPopup("Settings");
        }

        //Settings popup
        ImGui::SetNextWindowSize(ImVec2(350, 400));
        if(ImGui::BeginPopupModal("Settings"))
        {
            if(ImGui::CollapsingHeader("Keyboard settings"))
            {
                ImGui::Indent();
                for(std::size_t i = 0; i < 4; ++i)
                {
                    std::string label = "Player " + std::to_string(i + 1);
                    if(ImGui::CollapsingHeader(label.data()))
                    {
                        ImGui::Text("Left key: ");
                        ImGui::SameLine();
                        if(ImGui::Button(m_playersKeys[i][0]))
                        {
                            m_selectedKeyButton = m_playersKeys[i][0];
                        }

                        ImGui::Text("Right key: ");
                        ImGui::SameLine();
                        if(ImGui::Button(m_playersKeys[i][1]))
                        {
                            m_selectedKeyButton = m_playersKeys[i][1];
                        }

                        ImGui::Text("Jump key: ");
                        ImGui::SameLine();
                        if(ImGui::Button(m_playersKeys[i][2]))
                        {
                            m_selectedKeyButton = m_playersKeys[i][2];
                        }
                    }
                }
                ImGui::Unindent();
            }

            if(ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        //About
        if(ImGui::Button("About..."))
        {

        }

        //Quit button
        if(ImGui::Button("Exit"))
        {
            getStateEngine().stopStateAndUnpause();
        }
    ImGui::End();

    //Draw
    target.clear(sf::Color(140, 200, 255));

    target.draw(m_logoSprite);
    target.draw(m_groundSprite);
    target.draw(m_playerSprite);

    target.draw(m_boostLogo);
    target.draw(m_entityxLogo);
    target.draw(m_sfguiLogo);
    target.draw(m_sfmlLogo);
    target.draw(m_solLogo);
}

void MainMenuState::doStart()
{
    m_backgroundSound.play();
}

void MainMenuState::doStop()
{
    m_backgroundSound.stop();
}

void MainMenuState::doPause()
{
    m_backgroundSound.stop();
}

void MainMenuState::doUnpause()
{
    m_backgroundSound.play();
}

void MainMenuState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    float scale = 1.f + 0.05f * std::sin(getTimeSinceStart().asSeconds());
    m_logoSprite.setScale(sf::Vector2f(scale, scale));

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
    else //Move the player sprite back to its position after a drag
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
    }

    //Reset the animation to "default" when he's on the floor
    if(m_playerSprite.getPosition().y >= 370.f && m_playerSprite.getCurrentAnimation() != "default")
    {
        m_playerSprite.setCurrentAnimation("default");
    }
    else if(m_playerSprite.getPosition().y < 370.f && m_playerSprite.getCurrentAnimation() != "jump")
    {
        m_playerSprite.setCurrentAnimation("jump");
    }
}

void MainMenuState::updateKeysButtonsFromSettings()
{
    for(int i = 0; i < 4; i++)
    {
        settings::KeySettings::PlayerKeys& playerKeys = m_settingsManager.getKeySettings().getPlayerKeys(i);

        strncpy(m_playersKeys[i][0], tools::keyToString(playerKeys.leftKey).data(), 31);
        m_playersKeys[i][0][31] = '\0';

        strncpy(m_playersKeys[i][1], tools::keyToString(playerKeys.rightKey).data(), 31);
        m_playersKeys[i][1][31] = '\0';

        strncpy(m_playersKeys[i][2], tools::keyToString(playerKeys.jumpKey).data(), 31);
        m_playersKeys[i][2][31] = '\0';
    }
}

void MainMenuState::updateSettingsFromKeysButtons()
{
    for(int i = 0; i < 4; i++)
    {
        settings::KeySettings::PlayerKeys& playerKeys = m_settingsManager.getKeySettings().getPlayerKeys(i);

        playerKeys.leftKey = tools::stringToKey(std::string(m_playersKeys[i][0]));
        playerKeys.rightKey = tools::stringToKey(std::string(m_playersKeys[i][1]));
        playerKeys.jumpKey = tools::stringToKey(std::string(m_playersKeys[i][2]));
    }
}

}
