
namespace yapg
{

template<class T, typename... Args>
void StateEngine::stopAndStartState(Args&&... args, bool stopAll)
{
    m_todoNextFrameState.release();
    m_todoNextFrameState.reset(new T(*this, std::forward<Args>(args)...));

    m_todoNextFrame = [&]()
    {
        //If there is another state in the stack, stop it and pop it
        //(or clear the whole stack if stopAll==true)
        while(m_states.size() > 0)
        {
            m_states.top()->onStop();
            m_states.pop();

            if(!stopAll)
                break;
        }

        //Create the new state on top of the stack
        m_states.push(std::move(m_todoNextFrameState));
        m_states.top()->onStart();

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
            m_states.top()->onPause();
        }

        //Create the new state on top of the stack
        m_states.push(std::move(m_todoNextFrameState));
        m_states.top()->onStart();

        notify();
    };
}

}
