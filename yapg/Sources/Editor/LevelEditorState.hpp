#ifndef YAPG_GAME_STATE_LEVELEDITORSTATE_H
#define YAPG_GAME_STATE_LEVELEDITORSTATE_H

#include <map>
#include <memory>
#include <string>

#include "entityx/entityx.h"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "Level/Level.hpp"
#include "Lua/LuaState.hpp"
#include "Template/EntityTemplate.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "Editor/PropertiesManager.hpp"
#include "State/State.hpp"

namespace yapg{ class StateEngine; }

namespace yapg
{

class LevelEditorState : public State
{
public:
    LevelEditorState(StateEngine& stateEngine, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doStop();

    virtual void doPause();

    virtual void doUnpause();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    enum EditionMode
    {
        Insertion,
        Modify,
        SpawnConfig,
        Unknown
    };

    void initGUI();
    void initSystemManager();

    void newLevel();
    void openLevel();
    void saveLevel();
    void saveAsLevel();

    void updateGuiFromLevel();

    EditionMode getEditionMode() const;

    void updateTemplatesList();

    bool isMouseNotOnWidgets(sf::Vector2i mousePosition, sf::RenderTarget& target) const;

    entityx::Entity getFirstEntityUnderMouse(sf::Vector2i mousePosition, sf::RenderTarget& target);
    entityx::Entity getFirstEntityUnderMouse(sf::Vector2f position);
    bool isEntityUnderMouse(entityx::Entity entity, sf::Vector2i mousePosition, sf::RenderTarget& target) const;
    bool isEntityUnderMouse(entityx::Entity entity, sf::Vector2f position) const;

    sf::Vector2f getInsertionPosition(sf::Vector2f position, float entityWidth, float entityHeight, entityx::Entity ignore = entityx::Entity());

    sf::View getLevelView() const;

    AllResourcesManagers& m_resourcesManager;
    SettingsManager& m_settingsManager;

    LuaState m_luaState;

    sf::View m_guiView;

    EditionMode m_editionMode;

    std::vector<std::string> m_templatesNames;
    std::vector<std::string> m_templatesFriendlyNames;
    std::vector<sf::Texture> m_templatesTextures;
    std::size_t m_selectedTemplate;

    std::vector<std::string> m_playerTemplatesNames;
    std::string m_playerTemplatesNamesList;
    std::size_t m_selectedPlayerTemplate;

    PropertiesManager m_propertiesManager;

    std::unique_ptr<Level> m_level;
    std::string m_filepath;

    std::unique_ptr<entityx::SystemManager> m_systemMgr;

    //Future entity position in insertion mode
    bool m_isInserting;
    sf::Vector2f m_insertionPos;
    sf::Vector2i m_insertionCount;
    sf::Vector2f m_templateSize;

    //Entity selection stuff in "Modify" mode
    entityx::Entity m_selectedEntity;
    sf::Vector2f m_mouseOffsetToSelected;
    bool m_dragging;

    //Move the view stuff
    bool m_draggingView;
    sf::Vector2i m_mousePosBeforeDrag;

    //Spawn position
    sf::Sprite m_spawnSprite;
    std::shared_ptr<sf::Texture> m_spawnTexture;

    sf::Texture getIconFromTexture(sf::Texture texture) const;
    mutable sf::RenderTexture m_iconRenderTexture;
};

}

#endif
