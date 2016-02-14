#ifndef YAPG_GAME_STATE_MAINMENUSTATE_H
#define YAPG_GAME_STATE_MAINMENUSTATE_H

#include "simplgui/Button.h"
#include "simplgui/TextBox.h"

#include "Resources/GuiResourcesGetter.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace state
{

class MainMenuState : public State
{
public:
    MainMenuState(StateEngine& stateEngine, resources::TexturesManager& texturesManager, resources::FontManager& fontManager, settings::SettingsManager& settingsManager);

    virtual void onStart();

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void update(sf::Time dt);

    virtual void render(sf::RenderTarget& target);

private:
    resources::TexturesManager& m_texturesManager;
    resources::FontManager& m_fontManager;
    settings::SettingsManager& m_settingsManager;

    resources::GuiResourcesGetter::Ptr m_guiResGetter;

    //GUI Widgets
    simplgui::TextBox::Ptr m_levelPathTextBox;
    simplgui::Button::Ptr m_playLevelButton;

    simplgui::Button::Ptr m_settingsButton;
    simplgui::Button::Ptr m_quitButton;
};

}

#endif
