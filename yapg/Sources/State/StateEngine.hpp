#ifndef YAPG_GAME_STATE_STATEENGINE_H
#define YAPG_GAME_STATE_STATEENGINE_H

#include <deque>

#include "State/State.hpp"
#include "Tools/Observable.hpp"

namespace yapg{ class WindowManager; }

namespace yapg
{

class StateEngine : public Observable
{
public:
    StateEngine();

    template <class T, typename... Args>
    void stopAndStartState(Args&&... args);

    template <class T, typename... Args>
    void pauseAndStartState(Args&&... args);

    void stopStateAndUnpause();

    State::NonOwningPtr getRunningState();

    /**
     * Must be called at the beginning of each frame.
     * Stops and starts states if requested by states themselves in the previous
     * frame.
     */
    void nextFrameInit();

    WindowManager& getWindowManager() { return *m_windowManager; }
    const WindowManager& getWindowManager() const { return *m_windowManager; }

    void setWindowManager(WindowManager* windowManager) { m_windowManager = windowManager; }

private:
    std::deque<State::Ptr> m_states;

    std::function<void()> m_todoNextFrame;
    std::unique_ptr<State> m_todoNextFrameState;

    WindowManager* m_windowManager;
};

}

#include "StateEngine.inl"

#endif
