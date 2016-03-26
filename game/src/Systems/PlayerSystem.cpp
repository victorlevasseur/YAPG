#include "Systems/PlayerSystem.hpp"

#include "Components/PlatformerComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Settings/SettingsManager.hpp"
#include "Settings/KeySettings.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace c = components;

namespace systems
{

PlayerSystem::PlayerSystem(const settings::SettingsManager& settingsManager) :
    m_settingsManager(settingsManager)
{

}

void PlayerSystem::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt)
{
    es.each<c::PlayerComponent, c::PlatformerComponent>([&](entityx::Entity entity, c::PlayerComponent& player, c::PlatformerComponent& platformer)
    {
        //Get the keys associated to the player
        const settings::KeySettings::PlayerKeys& playerKeys =
            m_settingsManager.getKeySettings().getPlayerKeys(player.playerNumber);

        if(!player.finishedLevel)
        {
            platformer.wantsToGoLeft = sf::Keyboard::isKeyPressed(playerKeys.leftKey);
            platformer.wantsToGoRight = sf::Keyboard::isKeyPressed(playerKeys.rightKey);
            platformer.wantsToJump = sf::Keyboard::isKeyPressed(playerKeys.jumpKey);
        }
    });
}

}
