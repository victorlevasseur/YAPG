#ifndef YAPG_GAME_ASYNC_ASYNCEXECUTOR_H
#define YAPG_GAME_ASYNC_ASYNCEXECUTOR_H

#include <map>
#include <memory>

#include <SFML/System/Time.hpp>

#include "Async/Task.hpp"

namespace async
{

class AsyncExecutor
{
public:
    using TaskId = unsigned int;
    static const TaskId NO_TASK = static_cast<TaskId>(-1);

    AsyncExecutor();

    AsyncExecutor(const AsyncExecutor&) = delete;
    AsyncExecutor& operator=(const AsyncExecutor&) = delete;

    AsyncExecutor(AsyncExecutor&&) = delete;
    AsyncExecutor& operator=(AsyncExecutor&&) = delete;

    void update(sf::Time dt);

    template<typename TaskType, typename... Args>
    typename std::enable_if<std::is_base_of<Task, TaskType>::value, TaskId>::typeTaskId
    addNewTask(Args... args, sf::Time time, TaskId nextTo = NO_TASK)
    {
        m_pendingTasks[m_nextId] = PendingTask{
            std::make_unique<TaskType>(args...),
            time,
            nextTo
        };

        return m_nextId++;
    }

private:
    struct PendingTask
    {
        TaskId nextTo; ///< If not noTask (-1), the task will be executed after the task registered by this id.
        sf::Time t; ///< The duration to wait before executing the task.

        std::unique_ptr<Task> task; ///< The task to execute
    };

    std::map<TaskId, PendingTask> m_pendingTasks;
    TaskId m_nextId;
};

}

#endif
