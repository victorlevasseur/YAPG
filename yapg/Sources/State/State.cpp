#include "State/State.hpp"

#include "State/StateEngine.hpp"
#include "Window/WindowManager.hpp"

namespace yapg
{

State::State(StateEngine& stateEngine) :
    m_stateEngine(stateEngine)
{

}

void State::onStart()
{
    m_timeSinceStart = sf::Time();
    doStart();
}

void State::onStop()
{
    doStop();
}

void State::onPause()
{
    doPause();
}

void State::onUnpause()
{
    doUnpause();
}

void State::update(sf::Time dt, sf::RenderTarget &target)
{
    m_timeSinceStart += dt;
    doUpdate(dt, target);
}

void State::onError(const std::exception& e)
{
    throw e;
}

const StateEngine& State::getStateEngine() const
{
    return m_stateEngine;
}

StateEngine& State::getStateEngine()
{
    return m_stateEngine;
}

const tgui::Gui& State::getGui() const
{
    return m_stateEngine.getWindowManager().getGui();
}

tgui::Gui& State::getGui()
{
    return m_stateEngine.getWindowManager().getGui();
}

sf::Time State::getTimeSinceStart() const
{
    return m_timeSinceStart;
}

}
