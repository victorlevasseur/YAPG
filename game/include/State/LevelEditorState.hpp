#ifndef YAPG_GAME_STATE_LEVELEDITORSTATE_H
#define YAPG_GAME_STATE_LEVELEDITORSTATE_H

#include <map>
#include <memory>
#include <string>

#include "entityx/entityx.h"

#include <SFGUI/Box.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/ListBox.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/ToggleButton.hpp>
#include <SFGUI/Window.hpp>

#include "Level/Level.hpp"
#include "Lua/EntityTemplate.hpp"
#include "Lua/LuaState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/Editor/PropertiesManager.hpp"
#include "State/State.hpp"

namespace state{ class StateEngine; }

namespace state
{

class LevelEditorState : public State
{
public:
    LevelEditorState(StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop);

    virtual void onStop();

    virtual void onPause();

    virtual void onUnpause();

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    enum class EditionMode
    {
        Insertion,
        Modify,
        View,
        Unknown
    };

    void initGUI();
    void initSystemManager();

    EditionMode getEditionMode() const;

    void updateTemplatesList();

    bool isMouseNotOnWidgets(sf::Vector2i mousePosition, sf::RenderTarget& target) const;

    entityx::Entity getFirstEntityUnderMouse(sf::Vector2i mousePosition, sf::RenderTarget& target);
    bool isEntityUnderMouse(entityx::Entity entity, sf::Vector2i mousePosition, sf::RenderTarget& target) const;

    resources::AllResourcesManagers& m_resourcesManager;
    settings::SettingsManager& m_settingsManager;

    lua::LuaState m_luaState;

    sf::View m_guiView;
    sf::View m_levelView;

    sfg::SFGUI& m_sfgui;
    sfg::Desktop& m_desktop;
    sfg::Window::Ptr m_fileToolbar;
    sfg::Window::Ptr m_toolsToolbar;
        sfg::RadioButton::Ptr m_insertionTool;
        sfg::RadioButton::Ptr m_modifyTool;
        sfg::RadioButton::Ptr m_viewTool;
    sfg::Window::Ptr m_toolsSettingsToolbar;
        sfg::ListBox::Ptr m_templatesListBox;
        std::vector<std::string> m_templatesNames;
        sfg::ScrolledWindow::Ptr m_propertiesScrolled;
        std::unique_ptr<editor::PropertiesManager> m_propertiesManager;

    level::Level m_level;
    std::unique_ptr<entityx::SystemManager> m_systemMgr;

    entityx::Entity m_selectedEntity;
    sf::Vector2f m_mouseOffsetToSelected;
};

}

#endif
