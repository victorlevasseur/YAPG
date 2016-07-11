#ifndef YAPG_GAME_TOOLS_OBSERVER_H
#define YAPG_GAME_TOOLS_OBSERVER_H

namespace tools{ class Observable; }


namespace tools
{

class Observer
{
public:
    virtual void update(Observable& observable, void* value = nullptr) = 0;
};

}

#endif
