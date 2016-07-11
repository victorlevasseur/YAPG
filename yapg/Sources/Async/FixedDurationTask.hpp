#ifndef YAPG_GAME_ASYNC_FIXEDDURATIONTASK_H
#define YAPG_GAME_ASYNC_FIXEDDURATIONTASK_H

#include <functional>

#include "Async/Task.hpp"
#include "Lua/sol.hpp"

namespace yapg
{

template<typename StartFunctionType, typename EndFunctionType, typename UpdateFunctionType>
class FixedDurationTask : public Task
{
public:
    FixedDurationTask(
        sf::Time duration,
        StartFunctionType startFunction = StartFunctionType(),
        EndFunctionType endFunction = EndFunctionType(),
        UpdateFunctionType updateFunction = UpdateFunctionType()
    ) : m_duration(duration), m_timeSinceStart(), m_startFunction(startFunction), m_endFunction(endFunction), m_updateFunction(updateFunction)
    {

    }

    virtual void onStart()
    {
        m_startFunction();
    }

    virtual void onUpdate(sf::Time dt)
    {
        m_timeSinceStart += dt;
        m_updateFunction(dt);
    }

    virtual void onEnd()
    {
        m_endFunction();
    }

    virtual bool isFinished() const
    {
        return m_timeSinceStart > m_duration;
    }

private:
    sf::Time m_duration;
    sf::Time m_timeSinceStart;

    StartFunctionType m_startFunction;
    EndFunctionType m_endFunction;
    UpdateFunctionType m_updateFunction;
};

using FunctionFixedDurationTask = FixedDurationTask<std::function<void()>, std::function<void()>, std::function<void(sf::Time)>>;
using LuaFixedDurationTask = FixedDurationTask<sol::function, sol::function, sol::function>;

}

#endif
