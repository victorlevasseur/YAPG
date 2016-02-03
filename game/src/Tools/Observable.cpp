#include "Tools/Observable.hpp"

#include "Tools/Observer.hpp"


namespace tools
{

Observable::Observable() :
    m_observers{}
{

}

void Observable::addObserver(Observer& observer)
{
    m_observers.insert(&observer);
}

void Observable::removeObserver(Observer& observer)
{
    m_observers.erase(&observer);
}

void Observable::notify(void* value)
{
    for(Observer* o : m_observers)
        o->update(*this, value);
}

}
