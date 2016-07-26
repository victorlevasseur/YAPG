#include "Editor/LevelEditorState.hpp"

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

namespace yapg
{

LevelEditorState::LevelEditorState(StateEngine& stateEngine, AllResourcesManagers& resourcesManager, SettingsManager& settingsManager) :
    State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_luaState(),
    m_guiView(sf::FloatRect(0.f, 0.f, 1024.f, 768.f)),
    m_editionMode(EditionMode::Insertion),
    m_templatesNames(),
    m_templatesFriendlyNames(),
    m_templatesTextures(),
    m_selectedTemplate(std::string::npos),
    m_playerTemplatesNames(),
    m_selectedPlayerTemplate(),
    m_propertiesManager(),
    m_level(m_luaState, Level::LevelMode::EditMode),
    m_filepath(),
    m_systemMgr(nullptr),
    m_isInserting(false),
    m_insertionPos(),
    m_insertionCount(),
    m_templateSize(),
    m_selectedEntity(),
    m_mouseOffsetToSelected(),
    m_dragging(false),
    m_draggingView(false),
    m_mousePosBeforeDrag(),
    m_spawnSprite(),
    m_spawnTexture(resourcesManager.getTextures().requestResource("assets/editor/spawn.png")),
    m_iconRenderTexture()
{
    m_iconRenderTexture.create(48, 48);

    m_propertiesManager.registerPropertyWidget<float, EntryPropertyWidget<float>>();
    m_propertiesManager.registerPropertyWidget<int, EntryPropertyWidget<int>>();
    m_propertiesManager.registerPropertyWidget<unsigned int, EntryPropertyWidget<unsigned int>>();
    m_propertiesManager.registerPropertyWidget<std::string, EntryPropertyWidget<std::string, true>>();

    initSystemManager();
    initGUI();
    updateTemplatesList();

    m_spawnSprite.setTexture(*m_spawnTexture);

    newLevel();
}

void LevelEditorState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    if(getEditionMode() == EditionMode::Insertion)
    {
        m_selectedEntity = entityx::Entity();

        if(event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target))
        {
            if(m_selectedTemplate != std::string::npos)
            {
                //Get the template, its width and height (just to check it's available!)
                const EntityTemplate& selectedTemplate = m_luaState.getTemplate(m_templatesNames[m_selectedTemplate]);
                if(selectedTemplate.getComponentsTable().get<sol::object>("position").is<sol::table>())
                {
                    sol::table positionComponent = selectedTemplate.getComponentsTable().get<sol::object>("position").as<sol::table>();

                    if(positionComponent.get<sol::object>("width").is<float>() && positionComponent.get<sol::object>("height").is<float>())
                    {
                        m_isInserting = true;
                        m_insertionCount = sf::Vector2i(0, 0);
                    }
                }
            }

        }
        else if(event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), getLevelView());

            //When not in insertion process, move the insertion pos according to the mouse
            if(!m_isInserting)
            {
                if(m_selectedTemplate != std::string::npos)
                {
                    //Get the template, its width and height
                    const EntityTemplate& selectedTemplate = m_luaState.getTemplate(m_templatesNames[m_selectedTemplate]);
                    if(selectedTemplate.getComponentsTable().get<sol::object>("position").is<sol::table>())
                    {
                        sol::table positionComponent = selectedTemplate.getComponentsTable().get<sol::object>("position").as<sol::table>();

                        if(positionComponent.get<sol::object>("width").is<float>() && positionComponent.get<sol::object>("height").is<float>())
                        {
                            m_templateSize.x = positionComponent.get<sol::object>("width").as<float>();
                            m_templateSize.y = positionComponent.get<sol::object>("height").as<float>();

                            //Set the future insertion position
                            m_insertionPos = getInsertionPosition(mousePosition - m_templateSize / 2.f, m_templateSize.x, m_templateSize.y);
                        }
                    }
                }
            }
            //If in insertion process, see how many entities the user wants to insert from m_insertionPos to the mouse position
            else
            {
                m_insertionCount.x = (mousePosition.x - m_insertionPos.x) / m_templateSize.x - (mousePosition.x - m_insertionPos.x < 0.f ? 1 : 0);
                m_insertionCount.y = (mousePosition.y - m_insertionPos.y) / m_templateSize.y - (mousePosition.y - m_insertionPos.y < 0.f ? 1 : 0);
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased &&
            event.mouseButton.button == sf::Mouse::Left &&
            isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target))
        {
            if(m_isInserting)
            {
                //Insert the new entities when the mouse is released.
                if(m_selectedTemplate != std::string::npos)
                {
                    //Get the template of the entities to insert
                    const EntityTemplate& entityTemplate = m_luaState.getTemplate(m_templatesNames[m_selectedTemplate]);

                    //Insert all the new entities
                    for(int i = std::min(0, m_insertionCount.x); i <= std::max(0, m_insertionCount.x); ++i)
                    {
                        for(int j = std::min(0, m_insertionCount.y); j <= std::max(0, m_insertionCount.y); ++j)
                        {
                            entityx::Entity newEntity = m_level.createNewEntity(m_templatesNames[m_selectedTemplate]);

                            try
                            {
                                newEntity.component<TemplateComponent>()->parametersHelper.setParameter("x", m_insertionPos.x + i * m_templateSize.x);
                                newEntity.component<TemplateComponent>()->parametersHelper.setParameter("y", m_insertionPos.y + j * m_templateSize.y);
                            }
                            catch(std::exception& e)
                            {
                                std::cout << "[Editor/Warning] The template \"" << entityTemplate.getName() << "\" can't be inserted because it doesn't have numerical X and Y parameters !" << std::endl;
                                newEntity.destroy();
                            }
                        }
                    }
                }

                m_isInserting = false;
                m_insertionCount = sf::Vector2i(0, 0);
            }
        }
        else if(event.type == sf::Event::KeyPressed &&
            isMouseNotOnWidgets(sf::Mouse::getPosition(dynamic_cast<sf::RenderWindow&>(target)), target) &&
            event.key.code == sf::Keyboard::Delete)
        {
            //Remove the entity under the mouse
            entityx::Entity entityUnderMouse = getFirstEntityUnderMouse(sf::Mouse::getPosition(dynamic_cast<sf::RenderWindow&>(target)), target);
            if(entityUnderMouse)
            {
                entityUnderMouse.destroy();
            }
        }
    }
    else if(getEditionMode() == EditionMode::Modify)
    {
        if(event.type == sf::Event::MouseButtonPressed
            && isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target)
            && event.mouseButton.button == sf::Mouse::Left
            )
        {
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), getLevelView());

            m_selectedEntity = getFirstEntityUnderMouse(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target);
            m_propertiesManager.setCurrentEntity(m_selectedEntity);

            if(m_selectedEntity)
            {
                sf::Vector2f entityPos;
                entityPos.x = boost::any_cast<float>(m_selectedEntity.component<TemplateComponent>()->parametersHelper.getParameter("x"));
                entityPos.y = boost::any_cast<float>(m_selectedEntity.component<TemplateComponent>()->parametersHelper.getParameter("y"));

                m_mouseOffsetToSelected = mousePosition - entityPos;
                m_dragging = true;
            }
        }
        else if(event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), getLevelView());

            if(isMouseNotOnWidgets(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), target))
            {
                if(m_selectedEntity && m_dragging)
                {
                    sf::Vector2f newPosition = mousePosition - m_mouseOffsetToSelected;
                    if(m_selectedEntity.has_component<PositionComponent>())
                    {
                        auto posComponent = m_selectedEntity.component<PositionComponent>();
                        newPosition = getInsertionPosition(newPosition, posComponent->width, posComponent->height, m_selectedEntity);
                    }

                    m_selectedEntity.component<TemplateComponent>()->parametersHelper.setParameter("x", std::round(newPosition.x));
                    m_selectedEntity.component<TemplateComponent>()->parametersHelper.setParameter("y", std::round(newPosition.y));
                    m_propertiesManager.setCurrentEntity(m_selectedEntity);
                }
            }
        }
        else if(event.type == sf::Event::MouseButtonReleased)
        {
            m_dragging = false;
        }
        else if(event.type == sf::Event::KeyPressed &&
            m_selectedEntity &&
            event.key.code == sf::Keyboard::Delete)
        {
            //Remove the selected entity
            m_selectedEntity.destroy();
        }
    }
    else if(getEditionMode() == EditionMode::SpawnConfig)
    {
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            if(isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target))
            {
                sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), getLevelView());
                m_level.setSpawnPosition(mousePosition);
            }
        }
    }

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

void LevelEditorState::render(sf::RenderTarget& target)
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("New level"))
            {
                newLevel();
            }

            if(ImGui::MenuItem("Open..."))
            {
                openLevel();
            }

            if(ImGui::MenuItem("Save"))
            {
                saveLevel();
            }

            if(ImGui::MenuItem("Save as..."))
            {
                saveAsLevel();
            }

            if(ImGui::MenuItem("Exit"))
            {
                getStateEngine().stopStateAndUnpause();
            }

            ImGui::EndMenu();
        }
        ImGui::MenuItem(m_filepath == "" ? "(level not saved)" : m_filepath.data(), NULL, false, false);

        ImGui::EndMainMenuBar();
    }

    ImGui::SetNextWindowPos(ImVec2(0.f, 25.f));
    ImGui::Begin("Edition mode");
        ImGui::RadioButton("Insertion mode", reinterpret_cast<int*>(&m_editionMode), EditionMode::Insertion);
        ImGui::RadioButton("Modify mode", reinterpret_cast<int*>(&m_editionMode), EditionMode::Modify);
        ImGui::RadioButton("Spawn config.", reinterpret_cast<int*>(&m_editionMode), EditionMode::SpawnConfig);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(1024.f-320.f, 25.f));
    ImGui::SetNextWindowSize(ImVec2(320.f, 300.f));
    ImGui::Begin("Templates");
        for(std::size_t i = 0; i < m_templatesNames.size(); ++i)
        {
            ImGui::PushID(i);

            bool stylePushed = false;
            if(i == m_selectedTemplate)
            {
                stylePushed = true;
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.f));
            }
            if(ImGui::ImageButton(m_templatesTextures[i]))
            {
                m_selectedTemplate = i;
            }
            if(stylePushed)
                ImGui::PopStyleColor();

            ImGui::SameLine();

            if(i == m_selectedTemplate)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 1.f, 0.f, 1.f));
            ImGui::Text(m_templatesFriendlyNames[i].substr(0, 32).data());
            if(i == m_selectedTemplate)
                ImGui::PopStyleColor();

            ImGui::PopID();
        }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(1024.f-320.f, 325.f));
    ImGui::SetNextWindowSize(ImVec2(320.f, 300.f));
    ImGui::Begin("Properties");
        if(m_selectedEntity)
        {
            ImGui::Text("Entity ID: %d.%d", m_selectedEntity.id().index(), m_selectedEntity.id().version());
            m_propertiesManager.display();
        }
        else
        {
            ImGui::TextWrapped("Select an entity to display its properties here.");
        }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(1024.f-320.f, 625.f));
    ImGui::SetNextWindowSize(ImVec2(320.f, 100.f));
    ImGui::Begin("Spawn config.");
        ImGui::Text("P1 template :");
        ImGui::SameLine();
        if(ImGui::Combo("",
            reinterpret_cast<int*>(&m_selectedPlayerTemplate),
            m_playerTemplatesNamesList.data(),
            m_playerTemplatesNames.size()
        ))
        {
            m_level.getPlayersTemplates()[0] = (m_selectedPlayerTemplate != 0 ? m_playerTemplatesNames[m_selectedPlayerTemplate] : "");
        }

        ImGui::TextWrapped("To move the spawn, select \"Spawn config.\" in the \"Edition Mode\" window and click on the level.");
    ImGui::End();

    target.clear(sf::Color(0, 180, 255));

    //Render the level
    m_systemMgr->system<RenderSystem>()->render(target);

    //Draw the spawn position
    target.setView(m_systemMgr->system<RenderSystem>()->getView());
    m_spawnSprite.setPosition(m_level.getSpawnPosition());
    target.draw(m_spawnSprite);

    //Draw the selection box (or insertion box)
    if(getEditionMode() == EditionMode::Insertion)
    {
        sf::RectangleShape ghostRect(m_templateSize
            + sf::Vector2f(
                std::abs(static_cast<float>(m_insertionCount.x)) * m_templateSize.x,
                std::abs(static_cast<float>(m_insertionCount.y)) * m_templateSize.y
            )
        );
        ghostRect.setPosition(
            sf::Vector2f(
                m_insertionPos.x + std::min(static_cast<float>(m_insertionCount.x) * m_templateSize.x, 0.f),
                m_insertionPos.y + std::min(static_cast<float>(m_insertionCount.y) * m_templateSize.y, 0.f)
            )
        );
        ghostRect.setFillColor(sf::Color(0, 0, 255, 100));
        ghostRect.setOutlineThickness(1.f);
        ghostRect.setOutlineColor(sf::Color(0, 0, 255, 128));
        target.draw(ghostRect);
    }
    else if(getEditionMode() == EditionMode::Modify)
    {
        //Draw the selection box
        if(m_selectedEntity)
        {
            auto position = m_selectedEntity.component<PositionComponent>();
            sf::RectangleShape selectionRect(sf::Vector2f(position->width, position->height));
            selectionRect.setPosition(
                sf::Vector2f(position->x, position->y)
            );
            selectionRect.setFillColor(sf::Color(0, 0, 255, 100));
            selectionRect.setOutlineThickness(1.f);
            selectionRect.setOutlineColor(sf::Color(0, 0, 255, 128));
            target.draw(selectionRect);
        }
    }

    //Render the gui
    target.setView(m_guiView);
}

void LevelEditorState::doStart()
{

}

void LevelEditorState::doStop()
{

}

void LevelEditorState::doPause()
{

}

void LevelEditorState::doUnpause()
{

}

void LevelEditorState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    m_systemMgr->update<EntityGridSystem>(dt.asSeconds());
    m_systemMgr->update<RenderSystem>(dt.asSeconds());
}

void LevelEditorState::initGUI()
{

}

void LevelEditorState::initSystemManager()
{
    m_systemMgr.reset(new entityx::SystemManager(m_level.getEntityManager(), m_level.getEventManager()));

    m_systemMgr->add<EntityGridSystem>();

    auto& grid = m_systemMgr->system<EntityGridSystem>()->getGrid();
    m_systemMgr->add<RenderSystem>(m_resourcesManager.getTextures(), grid, false);
    m_systemMgr->system<RenderSystem>()->setView(m_guiView);

    m_systemMgr->configure();
}

void LevelEditorState::newLevel()
{
    m_level.loadFromFile("newlevel.xml");
    m_filepath = std::string();

    updateGuiFromLevel();
}

void LevelEditorState::openLevel()
{
    FileDialog fileDialog("Select a level to open...", FileDialog::Open, { { "XML levels", {"*.xml"} } });
    if(fileDialog.run())
    {
        m_level.loadFromFile(fileDialog.getFilename());
        m_filepath = fileDialog.getFilename();
        updateGuiFromLevel();
    }
}

void LevelEditorState::saveLevel()
{
    if(m_filepath.empty())
        saveAsLevel();
    else
        m_level.saveToFile(m_filepath);
}

void LevelEditorState::saveAsLevel()
{
    FileDialog fileDialog("Select where to save the level...", FileDialog::Save, { { "XML levels", {"*.xml"} } });
    if(fileDialog.run())
    {
        m_level.saveToFile(fileDialog.getFilename());
        m_filepath = fileDialog.getFilename();
    }
}

void LevelEditorState::updateGuiFromLevel()
{
    m_selectedPlayerTemplate = 0;
    if(m_level.getPlayersTemplates().size() > 0)
    {
        auto it = std::find(m_playerTemplatesNames.cbegin(), m_playerTemplatesNames.cend(), m_level.getPlayersTemplates()[0]);
        if(it != m_playerTemplatesNames.cend())
        {
            m_selectedPlayerTemplate = std::distance(m_playerTemplatesNames.cbegin(), it);
        }
    }
}

LevelEditorState::EditionMode LevelEditorState::getEditionMode() const
{
    return m_editionMode;
}

void LevelEditorState::updateTemplatesList()
{
    //Clear all buttons
    m_templatesNames.clear();
    m_templatesFriendlyNames.clear();
    m_templatesTextures.clear();
    m_playerTemplatesNames.clear();
    m_playerTemplatesNames.push_back("");
    m_playerTemplatesNamesList += std::string("Pas de joueur\0", 14);

    auto& templatesList = m_luaState.getTemplates();
    for(auto& templatePair : templatesList)
    {
        const auto& entityTemplate = templatePair.second;
        if(!entityTemplate.isAbstract())
        {
            //If it's a player, add it to the player templates list
            if(entityTemplate.isPlayer())
            {
                m_playerTemplatesNames.push_back(entityTemplate.getName());
                m_playerTemplatesNamesList += entityTemplate.getName() + '\0';
            }
            else //If not, add it to the general template list
            {
                m_templatesNames.push_back(entityTemplate.getName());
                m_templatesFriendlyNames.push_back(entityTemplate.getFriendlyName());
                sf::Texture entityTexture = entityTemplate.getTexture();
                m_templatesTextures.push_back(getIconFromTexture(entityTemplate.getTexture()));
            }
        }
    }

    m_playerTemplatesNamesList += '\0';
}

bool LevelEditorState::isMouseNotOnWidgets(sf::Vector2i mousePosition, sf::RenderTarget& target) const
{
    return (!ImGui::IsMouseHoveringAnyWindow());
}

entityx::Entity LevelEditorState::getFirstEntityUnderMouse(sf::Vector2i mousePosition, sf::RenderTarget& target)
{
    entityx::ComponentHandle<PositionComponent> position;
    for(entityx::Entity entity : m_level.getEntityManager().entities_with_components(position))
    {
        if(isEntityUnderMouse(entity, mousePosition, target))
            return entity;
    }

    return entityx::Entity();
}

entityx::Entity LevelEditorState::getFirstEntityUnderMouse(sf::Vector2f position)
{
    entityx::ComponentHandle<PositionComponent> positionComponent;
    for(entityx::Entity entity : m_level.getEntityManager().entities_with_components(positionComponent))
    {
        if(isEntityUnderMouse(entity, position))
            return entity;
    }

    return entityx::Entity();
}

bool LevelEditorState::isEntityUnderMouse(entityx::Entity entity, sf::Vector2i mousePosition, sf::RenderTarget& target) const
{
    sf::Vector2f mouseAbsolutePos = target.mapPixelToCoords(mousePosition, getLevelView());

    return isEntityUnderMouse(entity, mouseAbsolutePos);
}

bool LevelEditorState::isEntityUnderMouse(entityx::Entity entity, sf::Vector2f position) const
{
    auto positionComponent = entity.component<PositionComponent>();
    if(!positionComponent)
        return false;

    sf::FloatRect entityBoundingBox(positionComponent->x, positionComponent->y, positionComponent->width, positionComponent->height);

    return entityBoundingBox.contains(position);
}

namespace
{
    bool isNear(float pos, float compare, float threshold = 10.f)
    {
        return (pos >= compare - threshold && pos <= compare + threshold);
    }
}

sf::Vector2f LevelEditorState::getInsertionPosition(sf::Vector2f position, float entityWidth, float entityHeight, entityx::Entity ignore)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        return position;

    sf::Vector2f newPosition = position;

    entityx::ComponentHandle<PositionComponent> positionComponent;
    for(entityx::Entity entity : m_level.getEntityManager().entities_with_components(positionComponent))
    {
        if(ignore == entity)
            continue;

        sf::FloatRect otherEntityBoundingBox(positionComponent->x, positionComponent->y, positionComponent->width, positionComponent->height);

        if(newPosition.y + entityHeight >= otherEntityBoundingBox.top - 10.f && newPosition.y <= otherEntityBoundingBox.top + otherEntityBoundingBox.height + 10.f)
        {
            if(isNear(newPosition.x + entityWidth, otherEntityBoundingBox.left))
                newPosition.x = otherEntityBoundingBox.left - entityWidth;
            else if(isNear(newPosition.x, otherEntityBoundingBox.left))
                newPosition.x = otherEntityBoundingBox.left;
            else if(isNear(newPosition.x + entityWidth, otherEntityBoundingBox.left + otherEntityBoundingBox.width))
                newPosition.x = otherEntityBoundingBox.left + otherEntityBoundingBox.width - entityWidth;
            else if(isNear(newPosition.x, otherEntityBoundingBox.left + otherEntityBoundingBox.width))
                newPosition.x = otherEntityBoundingBox.left + otherEntityBoundingBox.width;
        }

        if(newPosition.x + entityWidth >= otherEntityBoundingBox.left - 10.f && newPosition.x <= otherEntityBoundingBox.left + otherEntityBoundingBox.width + 10.f)
        {
            if(isNear(newPosition.y + entityHeight, otherEntityBoundingBox.top))
                newPosition.y = otherEntityBoundingBox.top - entityHeight;
            else if(isNear(newPosition.y, otherEntityBoundingBox.top))
                newPosition.y = otherEntityBoundingBox.top;
            else if(isNear(newPosition.y + entityHeight, otherEntityBoundingBox.top + otherEntityBoundingBox.height))
                newPosition.y = otherEntityBoundingBox.top + otherEntityBoundingBox.height - entityHeight;
            else if(isNear(newPosition.y, otherEntityBoundingBox.top + otherEntityBoundingBox.height))
                newPosition.y = otherEntityBoundingBox.top + otherEntityBoundingBox.height;
        }
    }

    return newPosition;
}

sf::View LevelEditorState::getLevelView() const
{
    return m_systemMgr->system<RenderSystem>()->getView();
}

sf::Texture LevelEditorState::getIconFromTexture(sf::Texture texture) const
{
    m_iconRenderTexture.clear(sf::Color::Transparent);

    sf::Sprite icon(texture);
    icon.setScale(
        48.f / static_cast<float>(texture.getSize().x),
        48.f / static_cast<float>(texture.getSize().y)
    );

    m_iconRenderTexture.draw(icon);

    m_iconRenderTexture.display();

    return m_iconRenderTexture.getTexture();
}

}
