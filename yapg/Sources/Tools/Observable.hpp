#ifndef YAPG_GAME_TOOLS_OBSERVABLE_H
#define YAPG_GAME_TOOLS_OBSERVABLE_H

#include <set>

namespace yapg{ class Observer; }


namespace yapg
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
