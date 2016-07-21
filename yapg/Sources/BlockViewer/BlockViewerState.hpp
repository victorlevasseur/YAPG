#ifndef YAPG_BLOCKVIEWER_BLOCKVIEWERSTATE_H
#define YAPG_BLOCKVIEWER_BLOCKVIEWERSTATE_H

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

class BlockViewerState : public State
{
public:
    BlockViewerState(StateEngine& stateEngine, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager);

    virtual void processEvent(sf::Event event, sf::RenderTarget &target);

    virtual void render(sf::RenderTarget& target);

protected:
    virtual void doStart();

    virtual void doStop();

    virtual void doPause();

    virtual void doUnpause();

    virtual void doUpdate(sf::Time dt, sf::RenderTarget &target);

private:
    struct ErrorMessage
    {
        enum ErrorType
        {
            NOTE,
            WARNING,
            ERROR
        };

        ErrorType errorType;
        std::string errorMsg;
    };

    void openTemplate(const std::string& filepath);

    void initGUI();
    void initSystemManager();

    bool isMouseNotOnWidgets(sf::Vector2i mousePosition, sf::RenderTarget& target) const;

    sf::View getLevelView() const;

    AllResourcesManagers& m_resourcesManager;
    SettingsManager& m_settingsManager;

    LuaState m_luaState;

    sf::View m_guiView;

    bool m_draggingView;
    sf::Vector2i m_mousePosBeforeDrag;

    entityx::EventManager m_eventMgr;
    entityx::EntityManager m_entityMgr;
    std::unique_ptr<entityx::SystemManager> m_systemMgr;

    entityx::Entity m_block;

    std::string m_filepath;

    std::vector<ErrorMessage> m_errorMessages;
};

}

#endif
