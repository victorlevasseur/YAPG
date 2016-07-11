#include "Player/PlayerSystem.hpp"

#include "Messaging/LevelMessages.hpp"
#include "Platformer/PlatformerComponent.hpp"
#include "Player/PlayerComponent.hpp"
#include "Settings/SettingsManager.hpp"
#include "Settings/KeySettings.hpp"

#include <SFML/Window/Keyboard.hpp>



namespace yapg
{

PlayerSystem::PlayerSystem(std::size_t playersCount, const SettingsManager& settingsManager) :
    m_playersCount(playersCount),
    m_stillAliveCount(playersCount),
    m_finishedCount(0),
    m_settingsManager(settingsManager)
{

}

void PlayerSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<PlayerComponent, PlatformerComponent>([&](entityx::Entity entity, PlayerComponent& player, PlatformerComponent& platformer)
    {
        //Get the keys associated to the player
        const KeySettings::PlayerKeys& playerKeys =
            m_settingsManager.getKeySettings().getPlayerKeys(player.playerNumber);

        if(!player.hasFinishedLevel())
        {
            platformer.wantsToGoLeft = sf::Keyboard::isKeyPressed(playerKeys.leftKey);
            platformer.wantsToGoRight = sf::Keyboard::isKeyPressed(playerKeys.rightKey);
            platformer.wantsToJump = sf::Keyboard::isKeyPressed(playerKeys.jumpKey);
        }
        else
        {
            platformer.wantsToGoLeft = false;
            platformer.wantsToGoRight = false;
            platformer.wantsToJump = false;
        }
    });
}

void PlayerSystem::receive(const PlayerFinishedMessage& msg)
{
    auto entity = entityx::Entity(msg.player);

    if(entity.has_component<PlayerComponent>())
    {
        ++m_finishedCount;
        sendLevelMessages();
    }
}

void PlayerSystem::receive(const HealthKilledMessage& msg)
{
    auto entity = entityx::Entity(msg.entity);

    if(entity.has_component<PlayerComponent>())
    {
        --m_stillAliveCount;
        sendLevelMessages();
    }
}

void PlayerSystem::sendLevelMessages()
{
    if(m_stillAliveCount <= 0)
    {
        //All players are dead, send a AllPlayersLostMessage
        emit<messaging::AllPlayersLostMessage>();
    }
    else if(m_finishedCount >= m_stillAliveCount)
    {
        //All remaining players just finished the level, send a AllPlayersFinishedMessage
        emit<messaging::AllPlayersFinishedMessage>();
    }
}

}
