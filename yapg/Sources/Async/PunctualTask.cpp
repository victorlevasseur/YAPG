#include "Async/PunctualTask.hpp"

namespace yapg
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
