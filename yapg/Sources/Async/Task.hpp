#ifndef YAPG_GAME_ASYNC_TASK_H
#define YAPG_GAME_ASYNC_TASK_H

#include <functional>

#include <SFML/System/Time.hpp>

namespace yapg
{

class Task
{
public:
    Task();

    virtual void onStart() {};
    virtual void onUpdate(sf::Time dt) {};
    virtual void onEnd() {};

    virtual bool isFinished() const = 0;

private:
    std::function<void()> m_startFunction;
    std::function<void(float)> m_updateFunction;
    std::function<void()> m_endFunction;
};

}

#endif
