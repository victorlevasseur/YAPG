#ifndef YAPG_GAME_COMPONENTS_COMPONENT_H
#define YAPG_GAME_COMPONENTS_COMPONENT_H

#include <map>
#include <string>

namespace game
{
namespace components
{

class Component
{
public:
    Component(const std::string &name);
    virtual ~Component();

    virtual const std::string& getName() = 0;

private:

};

}
}

#endif
