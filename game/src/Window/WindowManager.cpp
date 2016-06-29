#include "Window/WindowManager.hpp"

#include <iostream>

#include <SFML/System/Clock.hpp>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"

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

    m_window.setFramerateLimit(60);

    ImGui::SFML::Init(m_window);
}

WindowManager::~WindowManager()
{
    ImGui::SFML::Shutdown();
}

void WindowManager::run()
{
    sf::Clock dtClock;
    sf::Time elapsedTime;

    while (m_window.isOpen())
    {
        m_engine.nextFrameInit();
        if(!m_engine.getRunningState())
        {
            m_window.close();
        }

        sf::Event event;
        while (m_window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            //Let the state process the event
            if(m_engine.getRunningState())
                m_engine.getRunningState()->processEvent(event, m_window);

            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        elapsedTime += dtClock.restart();
        ImGui::SFML::Update(elapsedTime - elapsedTime % sf::seconds(1.f/60.f)); //Update ImGui
        //Fix the timestep so that the game logic is refreshed every 1/60s (60 fps)
        while(elapsedTime >= sf::seconds(1.f/60.f))
        {
            if(m_engine.getRunningState())
            {
                m_engine.getRunningState()->update(sf::seconds(1.f/60.f), m_window);
            }

            elapsedTime -= sf::seconds(1.f/60.f);
        }

        //Render the state
        if(m_engine.getRunningState())
            m_engine.getRunningState()->render(m_window);
        else
            m_window.clear(sf::Color::Black);

        ImGui::Render();

        m_window.display();
    }
}

void WindowManager::update(tools::Observable& observable, void* value)
{

}

}
