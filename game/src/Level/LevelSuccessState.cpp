#include "Level/LevelSuccessState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>

#include "State/StateEngine.hpp"

namespace level
{

LevelSuccessState::LevelSuccessState(state::StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    state::State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_winMusic(m_resourcesManager.getSounds().requestResource("menu/bensound-happiness.ogg")),
    m_winSound(*m_winMusic),
    m_font(m_resourcesManager.getFonts().requestResource("LiberationSans.ttf")),
    m_winText("Level clear !", *m_font, 70u),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_window(nullptr)
{
    //Sound init
    m_winSound.setLoop(true);
    m_winSound.setVolume(20.f);

    //Text init
    m_winText.setPosition(sf::Vector2f(512.f - m_winText.getLocalBounds().width/2.f, 200.f));

    //GUI init
    m_window = sfg::Window::Create(sfg::Window::BACKGROUND|sfg::Window::SHADOW);
    auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    box->SetSpacing(5.f);

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

void LevelSuccessState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    m_desktop.HandleEvent(event);
}

void LevelSuccessState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(80, 255, 80));

    target.draw(m_winText);
    m_sfgui.Display(dynamic_cast<sf::RenderWindow&>(target));
}

void LevelSuccessState::doStart()
{
    m_winSound.play();

    m_window->Show(true);
    m_desktop.Add(m_window);
    m_desktop.Refresh();
}

void LevelSuccessState::doStop()
{
    m_winSound.stop();

    m_window->Show(false);
    m_desktop.Remove(m_window);
}

void LevelSuccessState::doUpdate(sf::Time dt, sf::RenderTarget& target)
{
    m_desktop.Update(dt.asSeconds());
}

}
