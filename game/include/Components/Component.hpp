#ifndef YAPG_GAME_COMPONENTS_COMPONENT_H
#define YAPG_GAME_COMPONENTS_COMPONENT_H

#include <map>
#include <string>

#include "entityx/entityx.h"
#include "selene.h"

namespace game
{
namespace components
{

class Component
{
public:
    Component();
    virtual ~Component();

    virtual std::string getName() const = 0;

    virtual void loadFromLua(sel::Selector& luaSelector) = 0;

    static void assignComponent(entityx::Entity entity, const std::string &component, sel::Selector& luaSelector);

private:
};

std::ostream& operator<<(std::ostream& stream, const Component& component);

}
}

#endif
