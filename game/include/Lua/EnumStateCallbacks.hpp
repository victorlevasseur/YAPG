#ifndef YAPG_GAME_LUA_MULTIPLESTATECALLBACKS_H
#define YAPG_GAME_LUA_MULTIPLESTATECALLBACKS_H

#include <array>
#include "Lua/sol.hpp"

namespace game
{
namespace lua
{

template<unsigned int StatesCount, typename StateEnum, typename... Args>
class EnumStateCallbacks
{
public:
    EnumStateCallbacks(StateEnum initialState) :
        m_currentState(initialState),
        m_needCall(true)
    {

    }

    StateEnum getState() const
    {
        return m_currentState;
    }

    void setState(StateEnum state)
    {
        if(state != m_currentState)
            m_needCall = true;
        m_currentState = state;
    }

    void callIfNeeded(Args... args)
    {
        if(!m_needCall)
            return;

        m_callbacks[static_cast<int>(m_currentState)].call(args...);

        m_needCall = false;
    }

    void registerCallback(StateEnum forState, sol::function& callback)
    {
        m_callbacks[static_cast<int>(forState)] = callback;
    }

private:
    StateEnum m_currentState;
    bool m_needCall;

    std::array<sol::function, StatesCount> m_callbacks;
};

}
}

#endif
