#ifndef YAPG_GAME_ASYNC_PUNCTUALTASK_H
#define YAPG_GAME_ASYNC_PUNCTUALTASK_H

#include <functional>

#include "Async/Task.hpp"

namespace async
{

class PunctualTask : public Task
{
public:
    PunctualTask(const std::function<void()>& function);

    virtual void onStart();

    virtual bool isFinished() const { return true; }

private:
    std::function<void()> m_function;
};

}

#endif
