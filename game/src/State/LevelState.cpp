#include "State/LevelState.hpp"

#include "Systems/RenderSystem.hpp"

namespace game
{
namespace state
{

LevelState::LevelState(const std::string& path) :
    State(),
    m_luaState(),
    m_level(path, m_luaState),
    m_systemMgr(m_level.getEntityManager(), m_level.getEventManager()),
    m_previousDt()
{
    m_systemMgr.add<systems::RenderSystem>();

    m_systemMgr.configure();
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
    m_previousDt = dt;
    //m_systemMgr.update<ASystemHere>(dt.asSeconds());
}

void LevelState::render(sf::RenderTarget& target)
{
    if(!m_systemMgr.system<systems::RenderSystem>()->getRenderTarget())
    {
        std::cout << "Set render target to render system" << std::endl;
        m_systemMgr.system<systems::RenderSystem>()->setRenderTarget(&target);
    }

    target.clear(sf::Color(0, 180, 255));
    m_systemMgr.update<systems::RenderSystem>(m_previousDt.asSeconds());
}

}
}
