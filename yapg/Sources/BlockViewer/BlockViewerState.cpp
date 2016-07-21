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
#include "Template/EntityTemplate.hpp"
#include "Template/TemplateComponent.hpp"
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
    m_filepath()
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
        ImGui::Text(m_filepath == "" ? "(no block loaded)" : m_filepath.data());
        if(ImGui::Button("Open...."))
        {
            FileDialog fileDialog("Select a template to open...", FileDialog::Open, { { "Lua template", {"*.lua"} } });
            if(fileDialog.run())
            {
                openTemplate(fileDialog.getFilename());
            }
        }
        if(ImGui::Button("Refresh"))
        {
            if(!m_filepath.empty())
                openTemplate(m_filepath);
        }
        if(ImGui::Button("Exit"))
        {
            getStateEngine().stopStateAndUnpause();
        }
    ImGui::End();

    ImGui::SetNextWindowSize(ImVec2(1024.f, 150.f));
    ImGui::SetNextWindowPos(ImVec2(0.f, 768.f - 150.f));
    ImGui::Begin("Error messages");
        for(const auto& error : m_errorMessages)
        {
            if(error.errorType == ErrorMessage::NOTE)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 1.f, 1.f));
                ImGui::Text("Note: ");
                ImGui::PopStyleColor();
            }
            else if(error.errorType == ErrorMessage::WARNING)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 0.f, 1.f));
                ImGui::Text("Warning: ");
                ImGui::PopStyleColor();
            }
            else if(error.errorType == ErrorMessage::ERROR)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
                ImGui::Text("Error: ");
                ImGui::PopStyleColor();
            }

            ImGui::SameLine();
            ImGui::TextWrapped(error.errorMsg.c_str());
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

void BlockViewerState::openTemplate(const std::string& filepath)
{
    m_errorMessages.clear();
    m_luaState.unloadAllTemplates();
    if(m_block)
        m_block.destroy();
    m_filepath = "";

    m_luaState.loadAllTemplates();

    try
    {
        //Try to load the template at filepath and get its name
        m_luaState.getState().script_file(filepath);
    }
    catch(const sol::error& e)
    {
        //An error in the loaded template
        m_errorMessages.push_back({ErrorMessage::ERROR, e.what()});
        return;
    }

    sol::table templateTable;
    try
    {
        templateTable = m_luaState.getState().get<sol::table>("entity_template");
    }
    catch(const sol::error& e)
    {
        //Can't find the entity_template variable (or it's not a table)
        m_errorMessages.push_back({ErrorMessage::ERROR, "Can't find the \"entity_template\" variable in the lua template file !"});
        return;
    }

    EntityTemplate blockTemplate(templateTable, "");

    if(!blockTemplate.getBaseTemplate().empty() && !m_luaState.hasTemplate(blockTemplate.getBaseTemplate()))
    {
        //The inherited template doesn't exist !
        m_errorMessages.push_back({ErrorMessage::ERROR, "The template \"" + blockTemplate.getBaseTemplate() + "\" (base template of the loaded block) doesn't exists (or is errored) !"});
        return;
    }
    blockTemplate.applyInheritance(m_luaState);

    m_block = m_entityMgr.create();

    blockTemplate.initializeEntity(m_block, SerializedEntityGetter());

    m_filepath = filepath;

    m_errorMessages.push_back({ErrorMessage::NOTE, "Block successfully loaded."});
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
