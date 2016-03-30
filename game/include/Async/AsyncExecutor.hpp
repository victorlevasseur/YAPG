#ifndef YAPG_GAME_ASYNC_ASYNCEXECUTOR_H
#define YAPG_GAME_ASYNC_ASYNCEXECUTOR_H

#include <map>

namespace async
{

class AsyncExecutor
{
public:
    using TaskId = unsigned int;

    AsyncExecutor();

    AsyncExecutor(const AsyncExecutor&) = delete;
    AsyncExecutor& operator=(const AsyncExecutor&) = delete;

    AsyncExecutor(AsyncExecutor&&) = delete;
    AsyncExecutor& operator=(AsyncExecutor&&) = delete;

private:
};

}

#endif
