
#include <TGUI/TGUI.hpp>

namespace yapg
{

template<class T, typename... Args>
void StateEngine::stopAndStartState(Args&&... args)
{
    m_todoNextFrameState.release();
    m_todoNextFrameState.reset(new T(*this, std::forward<Args>(args)...));

    m_todoNextFrame = [&]()
    {
        //If there is another state in the stack, stop it and pop it
        //(or clear the whole stack if stopAll==true)
        if(m_states.size() > 0)
        {
            m_states.back()->onStop();
            m_states.pop_back();
        }

        //Create the new state on top of the stack
        m_states.push_back(std::move(m_todoNextFrameState));
        m_states.back()->onStart();

        notify();
    };
}

template <class T, typename... Args>
void StateEngine::pauseAndStartState(Args&&... args)
{
    m_todoNextFrameState.release();
    m_todoNextFrameState.reset(new T(*this, std::forward<Args>(args)...));

    m_todoNextFrame = [&]()
    {
        //If there is at least a state in the stack, pause it
        if(m_states.size() > 0)
        {
            m_states.back()->onPause();
        }

        //Create the new state on top of the stack
        m_states.push_back(std::move(m_todoNextFrameState));
        m_states.back()->onStart();

        notify();
    };
}

}
