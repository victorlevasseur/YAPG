#include "Async/PunctualTask.hpp"

namespace async
{

PunctualTask::PunctualTask(const std::function<void()>& function) :
    m_function(function),
    m_finished(false)
{

}

void PunctualTask::onStart()
{
    m_function();
    m_finished = true;
}

}
