#ifndef YAPG_GAME_TOOLS_ITERATETOOLS_H
#define YAPG_GAME_TOOLS_ITERATETOOLS_H

#include <algorithm>

namespace yapg
{

namespace iterate_tools
{

/**
 * Call the unary predicate todo for each values in the container.
 * If the predicate returns true, keep the value in the container, otherwise
 * safely delete it and keep iterating
 */
template<class Container, class UnaryPredicate>
void safeForEach(Container& container, UnaryPredicate todo)
{
    for(auto it = std::begin(container); it != std::end(container); )
    {
        if(todo(*it))
            ++it;
        else
            it = container.erase(it);
    }
}

}

}

#endif
