#include "State/State.hpp"

#include "State/StateEngine.hpp"
#include "Window/WindowManager.hpp"

namespace yapg
{

State::State(StateEngine& stateEngine) :
    m_stateEngine(stateEngine),
    m_stateGui(m_stateEngine.getWindowManager().getGui(), m_stateEngine.getWindowManager().getGuiTheme())
{

}

void State::onStart()
{
    m_timeSinceStart = sf::Time();
    doStart();
    m_stateGui.onStart();
}

void State::onStop()
{
    m_stateGui.onStop();
    doStop();
}

void State::onPause()
{
    m_stateGui.onPause();
    doPause();
}

void State::onUnpause()
{
    doUnpause();
    m_stateGui.onUnpause();
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

const StateGui& State::getGui() const
{
    return m_stateGui;
}

StateGui& State::getGui()
{
    return m_stateGui;
}

sf::Time State::getTimeSinceStart() const
{
    return m_timeSinceStart;
}

}
