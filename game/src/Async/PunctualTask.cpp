#include "Async/PunctualTask.hpp"

namespace async
{

PunctualTask::PunctualTask(const std::function<void()>& function) :
    m_function(function)
{

}

void PunctualTask::onStart()
{
    m_function();
}

}
