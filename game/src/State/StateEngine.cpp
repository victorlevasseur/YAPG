#include "State/StateEngine.hpp"

namespace game
{
namespace state
{

StateEngine::StateEngine() :
    tools::Observable()
{

}

void StateEngine::stopStateAndUnpause()
{
    //If there is a top state, stop it
    if(m_states.size() > 1)
    {
        m_states.top()->onStop();
        m_states.pop();
    }

    //If there's still a state remaining, unpause it
    if(m_states.size() > 1)
    {
        m_states.top()->onUnpause();
    }

    notify();
}

State::NonOwningPtr StateEngine::getRunningState()
{
    if(m_states.size() > 0)
        return m_states.top().get();
    else
        return nullptr;
}

}
}
