#include "Health/HealthSystem.hpp"

#include "Common/PositionComponent.hpp"
#include "Health/HealthComponent.hpp"
#include "Messaging/LevelMessages.hpp"
#include "Platformer/PlatformComponent.hpp"
#include "Settings/SettingsManager.hpp"
#include "Settings/KeySettings.hpp"

#include <SFML/Window/Keyboard.hpp>



namespace yapg
{

HealthSystem::HealthSystem(const SettingsManager& settingsManager) :
    m_settingsManager(settingsManager),
    m_alreadyDeadEntities()
{

}

namespace
{
    float getLowestPlatformY(entityx::EntityManager &es)
    {
        float maxYPos = std::numeric_limits<float>::lowest();
        es.each<PositionComponent, PlatformComponent>([&](entityx::Entity, PositionComponent& position, PlatformComponent&)
        {
            if(position.y + position.height > maxYPos)
                maxYPos = position.y + position.height;
        });

        return maxYPos;
    }
}

void HealthSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    float lowestPlatformPos = getLowestPlatformY(es);

    //Test for entities beyond level's limits
    es.each<PositionComponent, HealthComponent>([&](entityx::Entity entity, PositionComponent& position, HealthComponent& health)
    {
        if(health.health <= 0)
            return;

        if(position.y > 200.f + lowestPlatformPos)
        {
            entity.component<HealthComponent>()->kill();
        }
    });
}

void HealthSystem::receive(const HealthKillMessage& msg)
{
    if(!entityx::Entity(msg.entityToKill).has_component<HealthComponent>())
        return;

    entityx::Entity(msg.entityToKill).component<HealthComponent>()->health = 0;
    m_alreadyDeadEntities.push_back(entityx::Entity(msg.entityToKill));

    emit<HealthKilledMessage>(msg.entityToKill);

    //TODO: Call the death callback or destroy the entity if no callback.
    if(false)
    {

    }
    else
    {
        entityx::Entity(msg.entityToKill).destroy();
    }
}

void HealthSystem::receive(const HealthLoosePVMessage& msg)
{
    if(!entityx::Entity(msg.entity).has_component<HealthComponent>())
        return;

    entityx::Entity(msg.entity).component<HealthComponent>()->health -= msg.pv;
    if(entityx::Entity(msg.entity).component<HealthComponent>()->health <= 0)
        entityx::Entity(msg.entity).component<HealthComponent>()->kill();

    //TODO: Call the loose PV callback

    //TODO: Set a invincibility period
}

void HealthSystem::receive(const HealthGainPVMessage& msg)
{
    if(!entityx::Entity(msg.entity).has_component<HealthComponent>())
        return;

    auto healthComp = entityx::Entity(msg.entity).component<HealthComponent>();
    healthComp->health += msg.pv;
    healthComp->health = std::min(healthComp->health, healthComp->maxHealth);

    //TODO: Call the gain PV callback
}

}
