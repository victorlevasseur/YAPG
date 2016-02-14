
namespace state
{

template<class T, typename... Args>
State::NonOwningPtr StateEngine::stopAndStartState(Args&&... args, bool stopAll)
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
    m_states.push(std::make_unique<T>(*this, std::forward<Args>(args)...));
    m_states.top()->onStart();

    notify();

    return m_states.top().get();
}

template <class T, typename... Args>
State::NonOwningPtr StateEngine::pauseAndStartState(Args&&... args)
{
    //If there is at least a state in the stack, pause it
    if(m_states.size() > 0)
    {
        m_states.top()->onPause();
    }

    //Create the new state on top of the stack
    m_states.push(std::make_unique<T>(*this, std::forward<Args>(args)...));
    m_states.top()->onUnpause();

    notify();

    return m_states.top().get();
}

}
