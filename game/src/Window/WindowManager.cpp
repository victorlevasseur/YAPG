#include "Window/WindowManager.hpp"

#include <iostream>

#include <SFML/System/Clock.hpp>

#include "State/State.hpp"
#include "State/StateEngine.hpp"


namespace window
{

WindowManager::WindowManager(state::StateEngine& engine, const sf::String& windowTitle) :
    tools::Observer(),
    m_window(sf::VideoMode(1024, 768), windowTitle),
    m_engine(engine)
{
    m_engine.addObserver(*this);
}

void WindowManager::run()
{
    sf::Clock dtClock;
    sf::Time elapsedTime;

    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            //Let the state process the event
            if(m_engine.getRunningState())
                m_engine.getRunningState()->processEvent(event);

            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        elapsedTime += dtClock.restart();
        //Fix the timestep so that the game logic is refreshed every 1/60s (60 fps)
        while(elapsedTime >= sf::seconds(1/60.f))
        {
            if(m_engine.getRunningState())
                m_engine.getRunningState()->update(sf::seconds(1/60.f));

            elapsedTime -= sf::seconds(1/60.f);
        }

        //Render the state
        if(m_engine.getRunningState())
            m_engine.getRunningState()->render(m_window);
        else
            m_window.clear(sf::Color::Black);

        m_window.display();
    }
}

void WindowManager::update(tools::Observable& observable, void* value)
{

}

}
