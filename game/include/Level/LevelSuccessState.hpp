#ifndef YAPG_GAME_LEVEL_LEVELSUCCESSSTATE_H
#define YAPG_GAME_LEVEL_LEVELSUCCESSSTATE_H

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Async/AsyncExecutor.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace level
{

class LevelSuccessState : public state::State
{
public:
    LevelSuccessState(state::StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doStop();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget& target);

private:
    resources::AllResourcesManagers& m_resourcesManager;

    std::shared_ptr<sf::SoundBuffer> m_winMusic;
    sf::Sound m_winSound;

    std::shared_ptr<sf::Font> m_font;
    sf::Text m_winText;
};

}

#endif
