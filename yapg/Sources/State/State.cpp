#include "State/State.hpp"

#include "State/StateEngine.hpp"
#include "Window/WindowManager.hpp"

namespace yapg
{

StateGui::StateGui(State& state, tgui::Gui& gui) :
    m_state(state),
    m_gui(gui)
{

}

State::State(StateEngine& stateEngine) :
    m_stateEngine(stateEngine),
    m_stateGui(*this, m_stateEngine.getWindowManager().getGui())
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
