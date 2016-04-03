#include "Level/LevelFailureState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>

#include "Level/LevelState.hpp"
#include "State/StateEngine.hpp"

namespace level
{

LevelFailureState::LevelFailureState(state::StateEngine& stateEngine, const std::string& levelPath, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    state::State(stateEngine),
    m_levelPath(levelPath),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_looseMusic(m_resourcesManager.getSounds().requestResource("menu/bensound-sadday.ogg")),
    m_looseSound(*m_looseMusic),
    m_font(m_resourcesManager.getFonts().requestResource("LiberationSans.ttf")),
    m_looseText("You failed !", *m_font, 70u),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_window(nullptr)
{
    //Sound init
    m_looseSound.setLoop(true);
    m_looseSound.setVolume(20.f);

    //Text init
    m_looseText.setPosition(sf::Vector2f(512.f - m_looseText.getLocalBounds().width/2.f, 200.f));

    //GUI init
    m_window = sfg::Window::Create(sfg::Window::BACKGROUND|sfg::Window::SHADOW);
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->SetSpacing(5.f);

    auto retryButton = sfg::Button::Create("Retry");
    retryButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        getStateEngine().stopAndStartState
        <level::LevelState, const std::string&, resources::AllResourcesManagers&, settings::SettingsManager&, sfg::SFGUI&, sfg::Desktop&>(
            m_levelPath, m_resourcesManager, m_settingsManager, m_sfgui, m_desktop
        );
    });

    box->PackEnd(retryButton);

    auto returnButton = sfg::Button::Create("Return to main menu");
    returnButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
    {
        getStateEngine().stopStateAndUnpause();
    });

    box->PackEnd(returnButton);
    m_window->Add(box);

    m_window->SetPosition(sf::Vector2f(
        512.f - m_window->GetAllocation().width/2.f,
        384.f - m_window->GetAllocation().height/2.f
    ));
    m_window->SetTitle("Main menu");
}

void LevelFailureState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    m_desktop.HandleEvent(event);
}

void LevelFailureState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(255, 80, 80));

    target.draw(m_looseText);
    m_sfgui.Display(dynamic_cast<sf::RenderWindow&>(target));
}

void LevelFailureState::doStart()
{
    m_looseSound.play();

    m_window->Show(true);
    m_desktop.Add(m_window);
    m_desktop.Refresh();
}

void LevelFailureState::doStop()
{
    m_looseSound.stop();

    m_window->Show(false);
    m_desktop.Remove(m_window);
}

void LevelFailureState::doUpdate(sf::Time dt, sf::RenderTarget& target)
{
    m_desktop.Update(dt.asSeconds());
}

}
