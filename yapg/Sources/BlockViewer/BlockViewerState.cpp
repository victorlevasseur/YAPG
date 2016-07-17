#include "BlockViewer/BlockViewerState.hpp"

#include <boost/filesystem.hpp>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Gui/imgui.h"
#include "Gui/imgui-SFML.h"

#include "Common/EntityGridSystem.hpp"
#include "Common/PositionComponent.hpp"
#include "Editor/EntryPropertyWidget.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/Template/EntityTemplate.hpp"
#include "Lua/Template/TemplateComponent.hpp"
#include "Menu/MainMenuState.hpp"
#include "NativeGui/FileDialog.hpp"
#include "NativeGui/TextBoxWindow.hpp"
#include "Rendering/RenderSystem.hpp"
#include "State/StateEngine.hpp"

namespace fs = boost::filesystem;

namespace yapg
{

BlockViewerState::BlockViewerState(StateEngine& stateEngine, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager) :
    State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_luaState(true),
    m_guiView(sf::FloatRect(0.f, 0.f, 1024.f, 768.f)),
    m_draggingView(false),
    m_mousePosBeforeDrag(),
    m_eventMgr(),
    m_entityMgr(m_eventMgr),
    m_block(),
    m_blockName(),
    m_blockToOpen{'\0'}
{
    initSystemManager();
    initGUI();
}

void BlockViewerState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    //View dragging
    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
    {
        sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
        if(isMouseNotOnWidgets(mousePos, target))
        {
            m_draggingView = true;
            m_mousePosBeforeDrag = mousePos;
        }
    }
    else if(event.type == sf::Event::MouseMoved)
    {
        sf::Vector2i mousePos(event.mouseMove.x, event.mouseMove.y);
        if(m_draggingView)
        {
            sf::View updatedView = m_systemMgr->system<RenderSystem>()->getView();
            updatedView.move(m_mousePosBeforeDrag.x - mousePos.x, m_mousePosBeforeDrag.y - mousePos.y);
            m_systemMgr->system<RenderSystem>()->setView(updatedView);
            m_mousePosBeforeDrag = mousePos;
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        m_draggingView = false;
    }
}

void BlockViewerState::render(sf::RenderTarget& target)
{
    ImGui::SetNextWindowSize(ImVec2(200.f, 150.f));
    ImGui::SetNextWindowPos(ImVec2(1024.f - 200.f, 0.f));
    ImGui::Begin("Files");
        ImGui::Text(m_blockName == "" ? "(no block loaded)" : m_blockName.data());
        if(ImGui::Button("Open...."))
        {
            ImGui::OpenPopup("Open");
        }
        if(ImGui::Button("Refresh"))
        {
            openTemplate(m_blockName);
        }
        if(ImGui::Button("Exit"))
        {
            getStateEngine().stopStateAndUnpause();
        }

        //Open a template POPUP
        if(ImGui::BeginPopupModal("Open"))
        {
            ImGui::Text("Template's name");

            ImGui::InputText("", m_blockToOpen, 127);

            if(ImGui::Button("Open"))
            {
                openTemplate(std::string(m_blockToOpen));
                ImGui::CloseCurrentPopup();
            }
            if(ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    ImGui::End();

    target.clear(sf::Color(0, 180, 255));

    //Render the level
    target.setView(m_guiView);
    m_systemMgr->system<RenderSystem>()->render(target);

    target.setView(m_guiView);
}

void BlockViewerState::doStart()
{

}

void BlockViewerState::doStop()
{

}

void BlockViewerState::doPause()
{

}

void BlockViewerState::doUnpause()
{

}

void BlockViewerState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    m_systemMgr->update<EntityGridSystem>(dt.asSeconds());
    m_systemMgr->update<RenderSystem>(dt.asSeconds());
}

void BlockViewerState::openTemplate(const std::string& name)
{
    m_luaState.unloadAllTemplates();
    if(m_block)
        m_block.destroy();
    m_blockName = "";

    m_luaState.loadAllTemplates();

    try
    {
        const auto& blockTemplate = m_luaState.getTemplate(name);
        m_block = m_entityMgr.create();

        blockTemplate.initializeEntity(m_block, SerializedEntityGetter(), true);

        m_blockName = name;
    }
    catch(const std::out_of_range& e)
    {
        if(m_block)
            m_block.destroy();

    }
}

void BlockViewerState::initGUI()
{

}

void BlockViewerState::initSystemManager()
{
    m_systemMgr.reset(new entityx::SystemManager(m_entityMgr, m_eventMgr));

    m_systemMgr->add<EntityGridSystem>();

    auto& grid = m_systemMgr->system<EntityGridSystem>()->getGrid();
    m_systemMgr->add<RenderSystem>(m_resourcesManager.getTextures(), grid, false);
    m_systemMgr->system<RenderSystem>()->setView(m_guiView);

    m_systemMgr->configure();
}

bool BlockViewerState::isMouseNotOnWidgets(sf::Vector2i mousePosition, sf::RenderTarget& target) const
{
    return (!ImGui::IsMouseHoveringAnyWindow());
}

sf::View BlockViewerState::getLevelView() const
{
    return m_systemMgr->system<RenderSystem>()->getView();
}

}
