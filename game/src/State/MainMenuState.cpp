#include "State/MainMenuState.hpp"

#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Table.hpp>

#include "Settings/KeySettings.hpp"
#include "State/LevelState.hpp"
#include "State/LevelEditorState.hpp"
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
    m_mainMenuWindow(),
    m_settingsWindow(),
    m_playersKeysWidgets(),
    m_lastSelectedKeyButton(),
    m_playerAnimations(),
    m_playerSprite(m_resourcesManager.getTextures().requestResource("menu/spritesheet_players.png"), m_playerAnimations),
    m_draggingPlayer(false),
    m_offsetToPlayer(0.f, 0.f),
    m_groundTexture(m_resourcesManager.getTextures().requestResource("menu/ground.png")),
    m_groundSprite(*m_groundTexture),
    m_backgroundSoundBuffer(m_resourcesManager.getSounds().requestResource("menu/bensound-clearday.ogg")),
    m_backgroundSound(*m_backgroundSoundBuffer)
{
    //Logo
    m_logoSprite.setOrigin(m_logoSprite.getLocalBounds().width/2.f, m_logoSprite.getLocalBounds().height/2.f);
    m_logoSprite.setPosition(sf::Vector2f(512.f, 120.f));

    //Main menu Window
    m_mainMenuWindow = sfg::Window::Create(sfg::Window::BACKGROUND|sfg::Window::SHADOW);
    auto windowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
    auto playBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 0.f);
    auto playLevelEntry = sfg::Entry::Create("level.lua");

    playLevelEntry->SetRequisition(sf::Vector2f(200.f, 0.f));
    playBox->PackEnd(playLevelEntry, true, true);

    auto playLevelButton = sfg::Button::Create("Play it!");
    playLevelButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind([&](sfg::Entry::PtrConst levelPathEntry)
    {
        getStateEngine().stopAndStartState
            <state::LevelState, std::string, resources::AllResourcesManagers&, settings::SettingsManager&>(
            levelPathEntry->GetText(), m_resourcesManager, m_settingsManager
        );
    }, playLevelEntry));
    playBox->PackEnd(playLevelButton, false, true);

    windowBox->PackEnd(playBox);

    auto editorButton = sfg::Button::Create("Level editor...");
    editorButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        getStateEngine().stopAndStartState
            <state::LevelEditorState, std::string, resources::AllResourcesManagers&, settings::SettingsManager&, sfg::SFGUI&, sfg::Desktop&>(
            "newlevel.lua", m_resourcesManager, m_settingsManager, m_sfgui, m_desktop
        );
    });
    windowBox->PackEnd(editorButton, true, true);

    auto settingsButton = sfg::Button::Create("Settings...");
    settingsButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        m_settingsWindow->Show(true);
        m_desktop.BringToFront(m_settingsWindow);
    });
    windowBox->PackEnd(settingsButton, true, true);

    auto quitButton = sfg::Button::Create("Exit");
    quitButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        getStateEngine().stopStateAndUnpause();
    });
    windowBox->PackEnd(quitButton);

    m_mainMenuWindow->Add(windowBox);

    m_mainMenuWindow->SetPosition(sf::Vector2f(
        512.f - m_mainMenuWindow->GetAllocation().width/2.f,
        300.f - m_mainMenuWindow->GetAllocation().height/2.f
    ));
    m_mainMenuWindow->SetTitle("Main menu");

    //Settings window
    m_settingsWindow = sfg::Window::Create(sfg::Window::TOPLEVEL|sfg::Window::SHADOW);
    m_settingsWindow->Show(false);

    auto settingsMainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    m_settingsWindow->Add(settingsMainBox);

    auto settingsNotebook = sfg::Notebook::Create();
    settingsMainBox->PackEnd(settingsNotebook);

    { //Keysettings tab
        auto keySettingsBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        keySettingsBox->PackEnd(sfg::Label::Create("Players keys:"), false, true);

        auto playersNotebook = sfg::Notebook::Create();
        keySettingsBox->PackEnd(playersNotebook, true, true);

        auto keyButtonClickCallback = [&](sfg::Button::Ptr button)
        {
            updateKeysButtonsFromSettings();
            button->SetLabel("Type to set");
            m_lastSelectedKeyButton = button;
        };

        for(int i = 0; i < 4; i++)
        {
            auto playerKeysTable = sfg::Table::Create();
            m_playersKeysWidgets.push_back(PlayerKeysWidgets{
                sfg::Button::Create("=="),
                sfg::Button::Create("=="),
                sfg::Button::Create("=="),
            });

            //Set callbacks
            m_playersKeysWidgets.back().leftKeyButton->GetSignal(sfg::Widget::OnLeftClick).Connect(
                std::bind(keyButtonClickCallback, m_playersKeysWidgets.back().leftKeyButton)
            );
            m_playersKeysWidgets.back().rightKeyButton->GetSignal(sfg::Widget::OnLeftClick).Connect(
                std::bind(keyButtonClickCallback, m_playersKeysWidgets.back().rightKeyButton)
            );
            m_playersKeysWidgets.back().jumpKeyButton->GetSignal(sfg::Widget::OnLeftClick).Connect(
                std::bind(keyButtonClickCallback, m_playersKeysWidgets.back().jumpKeyButton)
            );

            //Add the buttons
            playerKeysTable->Attach(sfg::Label::Create("Left: "), sf::Rect<sf::Uint32>(0, 0, 1, 1), sfg::Table::FILL);
            playerKeysTable->Attach(m_playersKeysWidgets.back().leftKeyButton, sf::Rect<sf::Uint32>(1, 0, 1, 1));
            playerKeysTable->Attach(sfg::Label::Create("Right: "), sf::Rect<sf::Uint32>(0, 1, 1, 1), sfg::Table::FILL);
            playerKeysTable->Attach(m_playersKeysWidgets.back().rightKeyButton, sf::Rect<sf::Uint32>(1, 1, 1, 1));
            playerKeysTable->Attach(sfg::Label::Create("Jump: "), sf::Rect<sf::Uint32>(0, 2, 1, 1), sfg::Table::FILL);
            playerKeysTable->Attach(m_playersKeysWidgets.back().jumpKeyButton, sf::Rect<sf::Uint32>(1, 2, 1, 1));

            playersNotebook->AppendPage(playerKeysTable, sfg::Label::Create("Player " + std::to_string(i+1)));
        }

        updateKeysButtonsFromSettings();

        settingsNotebook->AppendPage(keySettingsBox, sfg::Label::Create("Keyboard"));
    }

    auto backButton = sfg::Button::Create("Back");
    backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        m_settingsWindow->Show(false);
    });
    settingsMainBox->PackEnd(backButton);

    m_settingsWindow->SetPosition(sf::Vector2f(
        512.f - m_settingsWindow->GetAllocation().width/2.f,
        300.f - m_settingsWindow->GetAllocation().height/2.f
    ));
    m_settingsWindow->SetTitle("Settings");

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

    m_desktop.Remove(m_mainMenuWindow);
    m_desktop.Remove(m_settingsWindow);

    m_mainMenuWindow = nullptr; //Force SFGUI to dismiss those menus
    m_settingsWindow = nullptr;
}

void MainMenuState::onPause()
{
    m_mainMenuWindow->Show(false); //Hide them to be able to show them again when unpaused
    m_settingsWindow->Show(false);

    m_desktop.Remove(m_mainMenuWindow);
    m_desktop.Remove(m_settingsWindow);
    m_desktop.Refresh();
}

void MainMenuState::onUnpause()
{
    m_mainMenuWindow->Show(true); //Show again the main menu when unpaused

    m_desktop.Add(m_mainMenuWindow);
    m_desktop.Add(m_settingsWindow);
    m_desktop.Refresh();
}

void MainMenuState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    m_desktop.HandleEvent(event);

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
        if(m_lastSelectedKeyButton != nullptr)
        {
            if(event.key.code != sf::Keyboard::Escape)
                m_lastSelectedKeyButton->SetLabel(tools::keyToString(event.key.code));
            m_lastSelectedKeyButton = nullptr;
            updateSettingsFromKeysButtons();
            updateKeysButtonsFromSettings();
        }
    }
}

void MainMenuState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(140, 200, 255));

    target.draw(m_logoSprite);
    target.draw(m_groundSprite);
    target.draw(m_playerSprite);

    m_sfgui.Display(dynamic_cast<sf::RenderWindow&>(target));
}

void MainMenuState::doStart()
{
    m_backgroundSound.play();

    m_mainMenuWindow->Show(true);

    m_desktop.Add(m_mainMenuWindow);
    m_desktop.Add(m_settingsWindow);
}

void MainMenuState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    float scale = 1.f + 0.05f * std::sin(getTimeSinceStart().asSeconds());
    m_logoSprite.setScale(sf::Vector2f(scale, scale));

    m_desktop.Update(dt.asSeconds());

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

        m_playersKeysWidgets[i].leftKeyButton->SetLabel(tools::keyToString(playerKeys.leftKey));
        m_playersKeysWidgets[i].rightKeyButton->SetLabel(tools::keyToString(playerKeys.rightKey));
        m_playersKeysWidgets[i].jumpKeyButton->SetLabel(tools::keyToString(playerKeys.jumpKey));
    }
}

void MainMenuState::updateSettingsFromKeysButtons()
{
    for(int i = 0; i < 4; i++)
    {
        settings::KeySettings::PlayerKeys& playerKeys = m_settingsManager.getKeySettings().getPlayerKeys(i);

        playerKeys.leftKey = tools::stringToKey(m_playersKeysWidgets[i].leftKeyButton->GetLabel());
        playerKeys.rightKey = tools::stringToKey(m_playersKeysWidgets[i].rightKeyButton->GetLabel());
        playerKeys.jumpKey = tools::stringToKey(m_playersKeysWidgets[i].jumpKeyButton->GetLabel());
    }
}

}
