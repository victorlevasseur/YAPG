#ifndef YAPG_GAME_ASYNC_UNDEFINEDDURATIONTASK_H
#define YAPG_GAME_ASYNC_UNDEFINEDDURATIONTASK_H

#include <functional>

#include "Async/Task.hpp"
#include "Lua/sol.hpp"

namespace yapg
{

template<typename StartFunctionType, typename EndFunctionType, typename UpdateFunctionType>
class UndefinedDurationTask : public Task
{
public:
    UndefinedDurationTask(
        StartFunctionType startFunction = StartFunctionType(),
        EndFunctionType endFunction = EndFunctionType(),
        UpdateFunctionType updateFunction = UpdateFunctionType()
    ) : m_timeSinceStart(), m_startFunction(startFunction), m_endFunction(endFunction), m_updateFunction(updateFunction), m_finished(false)
    {

    }

    virtual void onStart()
    {
        m_startFunction();
    }

    virtual void onUpdate(sf::Time dt)
    {
        bool shouldFinish = false;
        m_updateFunction(dt, std::ref(shouldFinish));
        if(shouldFinish)
            m_finished = true;
    }

    virtual void onEnd()
    {
        m_endFunction();
    }

    virtual bool isFinished() const
    {
        return m_finished;
    }

private:
    StartFunctionType m_startFunction;
    EndFunctionType m_endFunction;
    UpdateFunctionType m_updateFunction;

    bool m_finished;
};

using FunctionUndefinedDurationTask = UndefinedDurationTask<std::function<void()>, std::function<void()>, std::function<void(sf::Time, bool&)>>;
using LuaUndefinedDurationTask = UndefinedDurationTask<sol::function, sol::function, sol::function>;

}

#endif
