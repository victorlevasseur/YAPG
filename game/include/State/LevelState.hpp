#ifndef YAPG_GAME_STATE_LEVELSTATE_H
#define YAPG_GAME_STATE_LEVELSTATE_H

#include <string>

#include "entityx/entityx.h"

#include "Lua/LuaState.hpp"
#include "State/State.hpp"


namespace game
{
namespace state
{

class LevelState : public State
{
public:
    LevelState(const std::string& path);
    
    virtual void onStart();
    
    virtual void onStop();
    
    virtual void onPause();
    
    virtual void onUnpause();
    
    virtual void processEvent(sf::Event event);
    
    virtual void update(sf::Time dt);
    
    virtual void render(sf::RenderTarget& target);
    
private:
    entityx::EventManager m_eventMgr;
    entityx::EntityManager m_entityMgr;
    entityx::SystemManager m_systemMgr;
    
    lua::LuaState m_luaState;
};

}
}

#endif