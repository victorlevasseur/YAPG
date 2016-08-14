#include "Window/WindowManager.hpp"

#include <iostream>

#include <SFML/System/Clock.hpp>

#include "Gui/imgui.h"
#include "Gui/imgui_internal.h"
#include "Gui/imgui-SFML.h"

#include "State/State.hpp"
#include "State/StateEngine.hpp"

namespace yapg
{

WindowManager::WindowManager(StateEngine& engine, const sf::String& windowTitle) :
    Observer(),
    m_window(sf::VideoMode(1024, 768), windowTitle),
    m_engine(engine),
    m_gui(m_window)
{
    m_engine.setWindowManager(this);
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

        try
        {
            sf::Event event;
            while (m_window.pollEvent(event))
            {
                ImGui::SFML::ProcessEvent(event);
                m_gui.handleEvent(event);

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

            m_gui.draw();
            ImGui::Render();

            m_window.display();
        }
        catch(const std::exception& e)
        {
            std::cout << "[Error] Exception uncaught during the game:" << std::endl;
            std::cout << e.what() << std::endl;

            if(m_engine.getRunningState())
                m_engine.getRunningState()->onError(e);
            else
                throw e;
        }
    }
}

void WindowManager::update(Observable& observable, void* value)
{

}

}
