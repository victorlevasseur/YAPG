#include "Systems/HealthSystem.hpp"

#include "Components/HealthComponent.hpp"
#include "Components/PlatformComponent.hpp"
#include "Components/PositionComponent.hpp"
#include "Messaging/LevelMessages.hpp"
#include "Settings/SettingsManager.hpp"
#include "Settings/KeySettings.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace c = components;

namespace systems
{

HealthSystem::HealthSystem(const settings::SettingsManager& settingsManager) :
    m_settingsManager(settingsManager),
    m_alreadyDeadEntities()
{

}

namespace
{
    float getLowestPlatformY(entityx::EntityManager &es)
    {
        float maxYPos = std::numeric_limits<float>::lowest();
        es.each<c::PositionComponent, c::PlatformComponent>([&](entityx::Entity, c::PositionComponent& position, c::PlatformComponent&)
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
    es.each<c::PositionComponent, c::HealthComponent>([&](entityx::Entity entity, c::PositionComponent& position, c::HealthComponent& health)
    {
        if(position.y > 200.f + lowestPlatformPos)
        {
            health.kill();
        }
    });

    //Test for dead entities (health <= 0)
    es.each<c::PositionComponent, c::HealthComponent>([&](entityx::Entity entity, c::PositionComponent& position, c::HealthComponent& health)
    {
        if(health.health <= 0 && std::find(m_alreadyDeadEntities.cbegin(), m_alreadyDeadEntities.cend(), entity) == m_alreadyDeadEntities.cend())
        {
            //TODO: Call the death callback or destroy the entity if no callback.
            if(false)
            {

            }
            else
            {
                entity.destroy();
            }

            //Remember the entity so as not to call the death callback again...
            m_alreadyDeadEntities.push_back(entity);
        }
    });
}

}
