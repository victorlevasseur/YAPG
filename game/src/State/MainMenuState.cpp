#include "State/MainMenuState.hpp"

#include "simplgui/Theme.h"

#include "State/LevelState.hpp"
#include "State/StateEngine.hpp"

namespace state
{

MainMenuState::MainMenuState(StateEngine& stateEngine, resources::TexturesManager& texturesManager, resources::FontManager& fontManager, settings::SettingsManager& settingsManager) :
    State(stateEngine),
    m_texturesManager(texturesManager),
    m_fontManager(fontManager),
    m_settingsManager(settingsManager),
    m_guiResGetter(resources::GuiResourcesGetter::create(m_fontManager)),
    m_levelPathTextBox(simplgui::TextBox::create(m_guiResGetter)),
    m_playLevelButton(simplgui::Button::create(m_guiResGetter)),
    m_settingsButton(simplgui::Button::create(m_guiResGetter)),
    m_quitButton(simplgui::Button::create(m_guiResGetter))
{
    simplgui::Theme theme = simplgui::Theme::defaultTheme();

    m_levelPathTextBox->setTheme(theme);
    m_playLevelButton->setTheme(theme);
    m_settingsButton->setTheme(theme);
    m_quitButton->setTheme(theme);

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
            "level.lua", m_texturesManager, m_settingsManager
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
}

void MainMenuState::onStart()
{

}

void MainMenuState::onStop()
{

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
}

void MainMenuState::update(sf::Time dt)
{
    m_levelPathTextBox->update(dt);
    m_playLevelButton->update(dt);
    m_settingsButton->update(dt);
    m_quitButton->update(dt);
}

void MainMenuState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(200, 200, 200));

    target.draw(*m_levelPathTextBox);
    target.draw(*m_playLevelButton);
    target.draw(*m_settingsButton);
    target.draw(*m_quitButton);
}

}
