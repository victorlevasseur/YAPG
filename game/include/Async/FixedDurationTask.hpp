#ifndef YAPG_GAME_ASYNC_FIXEDDURATIONTASK_H
#define YAPG_GAME_ASYNC_FIXEDDURATIONTASK_H

#include <functional>

#include "Async/Task.hpp"
#include "Lua/sol.hpp"

namespace async
{

template<typename FunctionType, typename... Args>
struct FunctionCaller
{
    static void call(FunctionType& function, Args... args)
    {

    }
};

template<typename... Args>
struct FunctionCaller<std::function<void(Args...)>, Args...>
{
    using FunctionType = std::function<void(Args...)>;

    static void call(FunctionType& function, Args... args)
    {
        function(std::forward(args)...);
    }
};

template<typename... Args>
struct FunctionCaller<sol::function, Args...>
{
    using FunctionType = sol::function;

    static void call(FunctionType& function, Args... args)
    {
        function(std::forward(args)...);
    }
};

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
        FunctionCaller<StartFunctionType>::call();
    }

    virtual void onUpdate(sf::Time dt)
    {
        m_timeSinceStart += dt;
        FunctionCaller<UpdateFunctionType, sf::Time>::call(dt);
    }

    virtual void onEnd()
    {
        FunctionCaller<EndFunctionType>::call();
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
using LuaFixedDurationTask = async::FixedDurationTask<sol::function, sol::function, sol::function>;

}

#endif
