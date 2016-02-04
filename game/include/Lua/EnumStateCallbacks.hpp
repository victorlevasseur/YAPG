#ifndef YAPG_GAME_LUA_MULTIPLESTATECALLBACKS_H
#define YAPG_GAME_LUA_MULTIPLESTATECALLBACKS_H

#include <array>
#include "Lua/sol.hpp"


namespace lua
{

/**
 * This class allows the game to register one callback per enum value and call it
 * when the enum changes its value.
 */
template<unsigned int StatesCount, typename StatesEnum, typename... Args>
class EnumStateCallbacks
{
public:
    EnumStateCallbacks(StatesEnum initialState) :
        m_currentState(initialState),
        m_needCall(true)
    {

    }

    StatesEnum getState() const
    {
        return m_currentState;
    }

    void setState(StatesEnum state)
    {
        if(state != m_currentState)
            m_needCall = true;
        m_currentState = state;
    }

    /**
     * Call the callback for the current value only if the value has changed.
     */
    void callIfNeeded(Args... args)
    {
        if(!m_needCall)
            return;

        m_callbacks[m_currentState].call(args...);

        m_needCall = false;
    }

    /**
     * Register a callback for one enum value.
     */
    void registerCallback(StatesEnum forState, sol::function& callback)
    {
        m_callbacks[forState] = callback;
    }

private:
    StatesEnum m_currentState;
    bool m_needCall;

    std::map<StatesEnum, sol::function> m_callbacks;
};

}

#endif
