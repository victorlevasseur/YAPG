#ifndef YAPG_GAME_TOOLS_OBSERVER_H
#define YAPG_GAME_TOOLS_OBSERVER_H

namespace yapg{ class Observable; }


namespace yapg
{

class Observer
{
public:
    virtual void update(Observable& observable, void* value = nullptr) = 0;
};

}

#endif
