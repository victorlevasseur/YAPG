#include "State/MainMenuState.hpp"

#include "simplgui/Theme.h"

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
    m_settingsButton(simplgui::Button::create(m_guiResGetter))
{
    simplgui::Theme theme = simplgui::Theme::defaultTheme();

    m_levelPathTextBox->setTheme(theme);
    m_playLevelButton->setTheme(theme);
    m_settingsButton->setTheme(theme);

    m_levelPathTextBox->setPosition(sf::Vector2f(100.f, 300.f));
    m_levelPathTextBox->setSize(sf::Vector2f(724.f, simplgui::AUTO_SIZE));
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

}

void MainMenuState::update(sf::Time dt)
{

}

void MainMenuState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(200, 200, 200));
}

}
