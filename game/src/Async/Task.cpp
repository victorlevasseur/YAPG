#include "Async/Task.hpp"

namespace async
{

Task::Task(const std::function<void()>& startFunction, float duration, const std::function<void(float)>& updateFunction, const std::function<void()>& endFunction) :
    m_startFunction(startFunction),
    m_updateFunction(updateFunction),
    m_endFunction(endFunction),
    m_duration(duration)
{

}

float Task::getDuration() const
{
    return m_duration;
}

void Task::onStart()
{
    if(m_startFunction)
        m_startFunction();
}

void Task::onUpdate(float dt)
{
    if(m_updateFunction)
        m_updateFunction(dt);
}

void Task::onEnd()
{
    if(m_endFunction)
        m_endFunction();
}

}
