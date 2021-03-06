#ifndef YAPG_GAME_LEVEL_LEVELFAILURESTATE_H
#define YAPG_GAME_LEVEL_LEVELFAILURESTATE_H

#include <string>

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Async/AsyncExecutor.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace yapg{ class StateEngine; }

namespace yapg
{

class LevelFailureState : public State
{
public:
    LevelFailureState(StateEngine& stateEngine, const std::string& levelPath, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doStop();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget& target);

private:
    std::string m_levelPath;

    AllResourcesManagers& m_resourcesManager;
    SettingsManager& m_settingsManager;

    std::shared_ptr<sf::SoundBuffer> m_looseMusic;
    sf::Sound m_looseSound;

    std::shared_ptr<sf::Font> m_font;
    sf::Text m_looseText;
};

}

#endif
