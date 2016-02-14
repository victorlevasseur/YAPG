#ifndef YAPG_GAME_STATE_MAINMENUSTATE_H
#define YAPG_GAME_STATE_MAINMENUSTATE_H

#include "Resources/GuiResourcesGetter.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state
{

class MainMenuState : public State
{
public:
    MainMenuState(resources::TexturesManager& texturesManager, resources::FontManager& fontManager, settings::SettingsManager& settingsManager);

    virtual void onStart();

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event);

    virtual void update(sf::Time dt);

    virtual void render(sf::RenderTarget& target);

private:
    resources::TexturesManager& m_texturesManager;
    resources::FontManager& m_fontManager;
    settings::SettingsManager& m_settingsManager;

    resources::GuiResourcesGetter::Ptr m_guiResGetter;
};

}

#endif
