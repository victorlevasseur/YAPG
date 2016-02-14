#include "State/State.hpp"

#include "State/StateEngine.hpp"

namespace state
{

State::State(StateEngine& stateEngine) :
    m_stateEngine(stateEngine)
{

}

const StateEngine& State::getStateEngine() const
{
    return m_stateEngine;
}

StateEngine& State::getStateEngine()
{
    return m_stateEngine;
}

}
