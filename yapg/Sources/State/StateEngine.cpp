#include "State/StateEngine.hpp"

#include <iostream>

namespace yapg
{

StateEngine::StateEngine() :
    Observable(),
    m_states(),
    m_todoNextFrame(),
    m_todoNextFrameState(nullptr)
{

}

void StateEngine::stopStateAndUnpause()
{
    m_todoNextFrameState.release();

    m_todoNextFrame = [&]()
    {
        //If there is a top state, stop it
        if(m_states.size() >= 1)
        {
            m_states.back()->onStop();
            m_states.pop_back();
        }

        //If there's still a state remaining, unpause it
        if(m_states.size() > 0)
        {
            m_states.back()->onUnpause();
        }

        notify();
    };
}

State::NonOwningPtr StateEngine::getRunningState()
{
    if(m_states.size() > 0)
        return m_states.back().get();
    else
        return nullptr;
}

void StateEngine::nextFrameInit()
{
    if(m_todoNextFrame)
        m_todoNextFrame();

    m_todoNextFrame = nullptr;
}

}
