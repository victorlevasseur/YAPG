#ifndef YAPG_GAME_TOOLS_OBSERVABLE_H
#define YAPG_GAME_TOOLS_OBSERVABLE_H

#include <set>

namespace tools{ class Observer; }


namespace tools
{

class Observable
{
public:
    Observable();

    void addObserver(Observer& observer);

    void removeObserver(Observer& observer);

    void notify(void* value = nullptr);

private:
    std::set<Observer*> m_observers;
};

}

#endif
