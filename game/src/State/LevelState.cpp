#include "State/LevelState.hpp"

namespace game
{
namespace state
{

LevelState::LevelState(const std::string& path) :
    State(),
    m_luaState(),
    m_level(path, m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager())
{

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
