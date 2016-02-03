#ifndef YAPG_GAME_STATE_STATEENGINE_H
#define YAPG_GAME_STATE_STATEENGINE_H

#include <stack>

#include "State/State.hpp"
#include "Tools/Observable.hpp"


namespace state
{

class StateEngine : public tools::Observable
{
public:
    StateEngine();

    template <class T, typename... Args>
    State::NonOwningPtr stopAndStartState(Args&&... args, bool stopAll = false);

    template <class T, typename... Args>
    State::NonOwningPtr pauseAndStartState(Args&&... args);

    void stopStateAndUnpause();

    State::NonOwningPtr getRunningState();

private:
    std::stack<State::Ptr> m_states;
};

}

#include "StateEngine.inl"

#endif
