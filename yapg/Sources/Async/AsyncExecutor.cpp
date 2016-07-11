#include "Async/AsyncExecutor.hpp"

#include <iostream>

namespace yapg
{

AsyncExecutor::AsyncExecutor() :
    m_pendingTasks(),
    m_nextId(0)
{

}

void AsyncExecutor::update(sf::Time dt)
{
    for(auto it = m_pendingTasks.begin(); it != m_pendingTasks.end(); ++it)
    {
        //Test if its previous task is finished
        if(it->second.nextTo != NO_TASK && (m_pendingTasks.count(it->second.nextTo) == 0 || m_pendingTasks.at(it->second.nextTo).task->isFinished()))
        {
            it->second.nextTo = NO_TASK;
        }

        //Update pending task remaining time and call onStart on those that have a remaining time < 0.
        if(it->second.nextTo == NO_TASK) //If the task doesn't have to wait the completion of other ones, decrease its remaining time
        {
            if(dt > it->second.t)
            {
                it->second.task->onStart();
            }

            it->second.t -= dt;
        }

        //Call onUpdate on all currently working tasks
        if(it->second.t <= sf::Time() && !it->second.task->isFinished())
        {
            it->second.task->onUpdate(dt);
        }
    }

    //Call onEnd on finished tasks and delete them
    for(auto it = m_pendingTasks.begin(); it != m_pendingTasks.end(); )
    {
        if(it->second.task->isFinished())
        {
            auto toDeleteIt = it++;
            toDeleteIt->second.task->onEnd();
            m_pendingTasks.erase(toDeleteIt);
        }
        else
        {
            ++it;
        }
    }
}

}
