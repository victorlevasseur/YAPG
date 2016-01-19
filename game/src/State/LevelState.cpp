#include "State/LevelState.hpp"

namespace game
{
namespace state
{

LevelState::LevelState(const std::string& path) :
    State(),
    m_eventMgr(),
    m_entityMgr(m_eventMgr),
    m_systemMgr(m_entityMgr, m_eventMgr),
    m_luaState()
{
    m_luaState.getState().Load(path);
}

void LevelState::onStart()
{
    
}

void LevelState::onStop()
{
    
}

void LevelState::onPause()
{
    
}

void LevelState::onUnpause()
{
    
}

void LevelState::processEvent(sf::Event event)
{
    
}

void LevelState::update(sf::Time dt)
{
    m_systemMgr.update_all(dt.asSeconds());
}

void LevelState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 180, 255));
}

}
}
