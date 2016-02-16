#include "State/State.hpp"

#include "State/StateEngine.hpp"

namespace state
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

void State::update(sf::Time dt, sf::RenderTarget &target)
{
    m_timeSinceStart += dt;
    doUpdate(dt, target);
}

const StateEngine& State::getStateEngine() const
{
    return m_stateEngine;
}

StateEngine& State::getStateEngine()
{
    return m_stateEngine;
}

sf::Time State::getTimeSinceStart() const
{
    return m_timeSinceStart;
}

}
