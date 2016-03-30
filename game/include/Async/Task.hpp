#ifndef YAPG_GAME_ASYNC_TASK_H
#define YAPG_GAME_ASYNC_TASK_H

#include <functional>

namespace async
{

class Task
{
public:
    Task(
        const std::function<void()>& startFunction,
        float duration = 0.f,
        const std::function<void(float)>& updateFunction = std::function<void(float)>(),
        const std::function<void()>& endFunction = std::function<void()>());

    float getDuration() const;

    void onStart();
    void onUpdate(float dt);
    void onEnd();

private:
    std::function<void()> m_startFunction;
    std::function<void(float)> m_updateFunction;
    std::function<void()> m_endFunction;
    float m_duration;
};

}

#endif
