#include "Editor/LevelEditorState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/RadioButtonGroup.hpp>
#include <SFGUI/Separator.hpp>

#include "Components/PositionComponent.hpp"
#include "Components/TemplateComponent.hpp"
#include "Editor/EntryPropertyWidget.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/EntityTemplate.hpp"
#include "NativeGui/FileDialog.hpp"
#include "NativeGui/TextBoxWindow.hpp"
#include "State/MainMenuState.hpp"
#include "State/StateEngine.hpp"
#include "Systems/RenderSystem.hpp"

namespace editor
{

LevelEditorState::LevelEditorState(state::StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    state::State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_luaState(),
    m_guiView(sf::FloatRect(0.f, 0.f, 1024.f, 768.f)),
    m_levelView(m_guiView),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_fileToolbar(),
    m_filepathLabel(),
    m_toolsToolbar(),
    m_toolsSettingsToolbar(),
    m_templatesListBox(),
    m_templatesNames(),
    m_propertiesScrolled(),
    m_propertiesManager(nullptr),
    m_playerTemplatesNames(),
    m_level(m_luaState, level::Level::LevelMode::EditMode),
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
    m_spawnTexture(resourcesManager.getTextures().requestResource("editor/spawn.png"))
{
    initSystemManager();
    initGUI();
    updateTemplatesList();

    m_spawnSprite.setTexture(*m_spawnTexture);

    newLevel();
}

void LevelEditorState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    m_desktop.HandleEvent(event);

    if(getEditionMode() == EditionMode::Insertion)
    {
        m_selectedEntity = entityx::Entity();

        if(event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left &&
            isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target))
        {
            if(m_templatesListBox->GetSelectedItemsCount() > 0)
            {
                //Get the template, its width and height (just to check it's available!)
                const lua::EntityTemplate& selectedTemplate = m_luaState.getTemplate(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()]);
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
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_levelView);

            //When not in insertion process, move the insertion pos according to the mouse
            if(!m_isInserting)
            {
                if(m_templatesListBox->GetSelectedItemsCount() > 0)
                {
                    //Get the template, its width and height
                    const lua::EntityTemplate& selectedTemplate = m_luaState.getTemplate(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()]);
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
                if(m_templatesListBox->GetSelectedItemsCount() > 0)
                {
                    //Get the template of the entities to insert
                    const lua::EntityTemplate& entityTemplate = m_luaState.getTemplate(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()]);

                    //Insert all the new entities
                    for(int i = std::min(0, m_insertionCount.x); i <= std::max(0, m_insertionCount.x); ++i)
                    {
                        for(int j = std::min(0, m_insertionCount.y); j <= std::max(0, m_insertionCount.y); ++j)
                        {
                            entityx::Entity newEntity = m_level.createNewEntity(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()], true);

                            try
                            {
                                newEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("x", m_insertionPos.x + i * m_templateSize.x);
                                newEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("y", m_insertionPos.y + j * m_templateSize.y);
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
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_levelView);

            m_selectedEntity = getFirstEntityUnderMouse(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target);
            m_propertiesManager->setCurrentEntity(m_selectedEntity);

            if(m_selectedEntity)
            {
                sf::Vector2f entityPos;
                entityPos.x = boost::any_cast<float>(m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.getParameter("x"));
                entityPos.y = boost::any_cast<float>(m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.getParameter("y"));

                m_mouseOffsetToSelected = mousePosition - entityPos;
                m_dragging = true;
            }
        }
        else if(event.type == sf::Event::MouseMoved)
        {
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_levelView);

            if(isMouseNotOnWidgets(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), target))
            {
                if(m_selectedEntity && m_dragging)
                {
                    sf::Vector2f newPosition = mousePosition - m_mouseOffsetToSelected;
                    if(m_selectedEntity.has_component<components::PositionComponent>())
                    {
                        auto posComponent = m_selectedEntity.component<components::PositionComponent>();
                        newPosition = getInsertionPosition(newPosition, posComponent->width, posComponent->height, m_selectedEntity);
                    }

                    m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("x", std::round(newPosition.x));
                    m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("y", std::round(newPosition.y));
                    m_propertiesManager->setCurrentEntity(m_selectedEntity);
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
                sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_levelView);
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
            m_levelView.move(m_mousePosBeforeDrag.x - mousePos.x, m_mousePosBeforeDrag.y - mousePos.y);
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
    target.clear(sf::Color(0, 180, 255));

    //Render the level
    target.setView(m_levelView);
    m_systemMgr->system<systems::RenderSystem>()->render(target);

    //Draw the spawn position
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
            auto position = m_selectedEntity.component<components::PositionComponent>();
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
    m_sfgui.Display(dynamic_cast<sf::RenderWindow&>(target));
}

void LevelEditorState::doStart()
{
    m_desktop.Add(m_fileToolbar);
    m_desktop.Add(m_toolsToolbar);
    m_desktop.Add(m_toolsSettingsToolbar);
}

void LevelEditorState::doStop()
{
    m_desktop.Remove(m_fileToolbar);
    m_desktop.Remove(m_toolsToolbar);
    m_desktop.Remove(m_toolsSettingsToolbar);
}

void LevelEditorState::doPause()
{
    m_fileToolbar->Show(false);
    m_toolsToolbar->Show(false);
    m_toolsSettingsToolbar->Show(false);
}

void LevelEditorState::doUnpause()
{
    m_fileToolbar->Show(true);
    m_toolsToolbar->Show(true);
    m_toolsSettingsToolbar->Show(true);
}

void LevelEditorState::doUpdate(sf::Time dt, sf::RenderTarget &target)
{
    m_systemMgr->update<systems::RenderSystem>(dt.asSeconds());
    m_desktop.Update(dt.asSeconds());
}

void LevelEditorState::initGUI()
{
    //FILE TOOLBAR
    m_fileToolbar = sfg::Window::Create(sfg::Window::BACKGROUND);
    {
        auto verticalFileBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        verticalFileBox->SetSpacing(5.f);
        m_fileToolbar->Add(verticalFileBox);

        m_filepathLabel = sfg::Label::Create("(Not saved yet)");
        verticalFileBox->PackEnd(m_filepathLabel);

        auto fileBox = sfg::Box::Create();
        fileBox->SetSpacing(5.f);
        verticalFileBox->PackEnd(fileBox);

        auto newButton = sfg::Button::Create("New level");
        fileBox->PackEnd(newButton);
        newButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&LevelEditorState::newLevel, this));

        auto openButton = sfg::Button::Create("Open level...");
        fileBox->PackEnd(openButton);
        openButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&LevelEditorState::openLevel, this));

        auto saveButton = sfg::Button::Create("Save");
        fileBox->PackEnd(saveButton);
        saveButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&LevelEditorState::saveLevel, this));

        auto saveAsButton = sfg::Button::Create("Save as...");
        fileBox->PackEnd(saveAsButton);
        saveAsButton->GetSignal(sfg::Widget::OnLeftClick).Connect(std::bind(&LevelEditorState::saveAsLevel, this));

        fileBox->PackEnd(sfg::Separator::Create(sfg::Separator::Orientation::VERTICAL));

        auto backButton = sfg::Button::Create("Return to menu");
        backButton->GetSignal(sfg::Widget::OnLeftClick).Connect([&]()
        {
            getStateEngine().stopStateAndUnpause();
        });
        fileBox->PackEnd(backButton);
    }

    //TOOLS TOOLBAR
    m_toolsToolbar = sfg::Window::Create(sfg::Window::BACKGROUND|sfg::Window::TITLEBAR);
    m_toolsToolbar->SetTitle("Tools");
    m_toolsToolbar->SetPosition(sf::Vector2f(
        0.f,
        m_fileToolbar->GetAllocation().top + m_fileToolbar->GetAllocation().height
    ));
    m_toolsToolbar->SetRequisition(sf::Vector2f(
        300.f,
        0.f
    ));

    auto toolsBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    m_toolsToolbar->Add(toolsBox);
    toolsBox->SetSpacing(2.f);

    auto radioGroup = sfg::RadioButtonGroup::Create();

    m_insertionTool = sfg::RadioButton::Create("Insert new entities", radioGroup);
    m_modifyTool = sfg::RadioButton::Create("Modify placed entities", radioGroup);
    m_spawnConfigTool = sfg::RadioButton::Create("Set spawn configuration", radioGroup);

    m_insertionTool->SetActive(true);
    toolsBox->PackEnd(m_insertionTool);
    toolsBox->PackEnd(m_modifyTool);
    toolsBox->PackEnd(m_spawnConfigTool);

    //TEMPLATES TOOLBAR
    m_toolsSettingsToolbar = sfg::Window::Create(sfg::Window::BACKGROUND|sfg::Window::TITLEBAR);
    m_toolsSettingsToolbar->SetTitle("Templates");
    m_toolsSettingsToolbar->SetPosition(sf::Vector2f(
        0.f,
        m_toolsToolbar->GetAllocation().top + m_toolsToolbar->GetAllocation().height
    ));
    m_toolsSettingsToolbar->SetRequisition(sf::Vector2f(
        300.f,
        400.f
    ));

    auto toolsSettingsBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    m_toolsSettingsToolbar->Add(toolsSettingsBox);

    m_templatesListBox = sfg::ListBox::Create();
    m_templatesListBox->SetItemTextPolicy(sfg::ListBox::ItemTextPolicy::SHRINK);
    toolsSettingsBox->PackEnd(m_templatesListBox);

    m_propertiesScrolled = sfg::ScrolledWindow::Create();
    m_propertiesScrolled->Show(false);
    toolsSettingsBox->PackEnd(m_propertiesScrolled);

    m_spawnConfigBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    m_spawnConfigBox->SetSpacing(5.f);
    m_spawnConfigBox->Show(false);
    toolsSettingsBox->PackEnd(m_spawnConfigBox);
    {
        m_spawnConfigBox->PackEnd(sfg::Label::Create("Left click on the level to\nset the spawn position."), false, true);

        m_spawnConfigTable = sfg::Table::Create();
        m_spawnConfigBox->PackEnd(m_spawnConfigTable, false, true);

        m_spawnConfigTable->Attach(sfg::Label::Create("Player template: "), sf::Rect<sf::Uint32>(0u, 0u, 1u, 1u), sfg::Table::FILL, sfg::Table::FILL);

        m_playerTemplateComboBox = sfg::ComboBox::Create();
        m_playerTemplateComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect([&]()
        {
            //Set the player template
            std::string newPlayerTemplate = m_playerTemplatesNames[m_playerTemplateComboBox->GetSelectedItem()];
            if(m_level.getPlayersTemplates().size() < 1)
                m_level.getPlayersTemplates().push_back(newPlayerTemplate);
            else
                m_level.getPlayersTemplates()[0] = newPlayerTemplate;
        });
        m_spawnConfigTable->Attach(m_playerTemplateComboBox, sf::Rect<sf::Uint32>(1u, 0u, 1u, 1u), sfg::Table::EXPAND|sfg::Table::FILL, sfg::Table::FILL);
    }

    //END OF TOOLS TOOLBAR
    auto enableCorrectToolSettings = [&](std::weak_ptr<sfg::Widget> toEnable, sf::String title)
    {
        m_templatesListBox->Show(false);
        m_propertiesScrolled->Show(false);
        m_spawnConfigBox->Show(false);

        m_toolsSettingsToolbar->SetTitle(title);

        if(toEnable.lock())
            toEnable.lock()->Show(true);
    };

    m_insertionTool->GetSignal(sfg::ToggleButton::OnToggle).Connect(
        std::bind(enableCorrectToolSettings, m_templatesListBox, "Templates")
    );

    m_modifyTool->GetSignal(sfg::ToggleButton::OnToggle).Connect(
        std::bind(enableCorrectToolSettings, m_propertiesScrolled, "Properties")
    );

    m_spawnConfigTool->GetSignal(sfg::ToggleButton::OnToggle).Connect(
        std::bind(enableCorrectToolSettings, m_spawnConfigBox, "Spawn configuration")
    );

    //Init the properties manager
    m_propertiesManager.reset(new PropertiesManager(m_propertiesScrolled));
    m_propertiesManager->registerPropertyWidget<float, editor::EntryPropertyWidget<float>>();
    m_propertiesManager->registerPropertyWidget<int, editor::EntryPropertyWidget<int>>();
    m_propertiesManager->registerPropertyWidget<unsigned int, editor::EntryPropertyWidget<unsigned int>>();
    m_propertiesManager->registerPropertyWidget<std::string, editor::EntryPropertyWidget<std::string, true>>();
}

void LevelEditorState::initSystemManager()
{
    m_systemMgr.reset(new entityx::SystemManager(m_level.getEntityManager(), m_level.getEventManager()));

    m_systemMgr->add<systems::RenderSystem>(m_resourcesManager.getTextures(), nullptr, true);

    m_systemMgr->configure();
}

void LevelEditorState::newLevel()
{
    m_level.LoadFromFile("newlevel.xml");
    m_filepath = std::string();

    m_filepathLabel->SetText("(not saved yet)");
    updateGuiFromLevel();
}

void LevelEditorState::openLevel()
{
    nativegui::FileDialog fileDialog("Select a level to open...", nativegui::FileDialog::Open, { { "XML levels", {"*.xml"} } });
    if(fileDialog.run())
    {
        m_level.LoadFromFile(fileDialog.getFilename());
        m_filepath = fileDialog.getFilename();

        m_filepathLabel->SetText(m_filepath);
        updateGuiFromLevel();
    }
}

void LevelEditorState::saveLevel()
{
    if(m_filepath.empty())
        saveAsLevel();
    else
        m_level.SaveToFile(m_filepath);
}

void LevelEditorState::saveAsLevel()
{
    nativegui::FileDialog fileDialog("Select where to save the level...", nativegui::FileDialog::Save, { { "XML levels", {"*.xml"} } });
    if(fileDialog.run())
    {
        m_level.SaveToFile(fileDialog.getFilename());
        m_filepath = fileDialog.getFilename();

        m_filepathLabel->SetText(m_filepath);
    }
}

void LevelEditorState::updateGuiFromLevel()
{
    m_playerTemplateComboBox->SelectItem(0);
    if(m_level.getPlayersTemplates().size() > 0)
    {
        auto it = std::find(m_playerTemplatesNames.cbegin(), m_playerTemplatesNames.cend(), m_level.getPlayersTemplates()[0]);
        if(it != m_playerTemplatesNames.cend())
        {
            m_playerTemplateComboBox->SelectItem(std::distance(m_playerTemplatesNames.cbegin(), it));
        }
    }
}

LevelEditorState::EditionMode LevelEditorState::getEditionMode() const
{
    if(m_insertionTool->IsActive())
        return EditionMode::Insertion;
    else if(m_modifyTool->IsActive())
        return EditionMode::Modify;
    else if(m_spawnConfigTool->IsActive())
        return EditionMode::SpawnConfig;
    return EditionMode::Unknown;
}

void LevelEditorState::updateTemplatesList()
{
    //Clear all buttons
    m_templatesListBox->Clear();
    m_playerTemplateComboBox->Clear();
    m_playerTemplateComboBox->AppendItem("No player!");
    m_templatesNames.clear();
    m_playerTemplatesNames.clear();
    m_playerTemplatesNames.push_back("");

    auto& templatesList = m_luaState.getTemplates();
    for(auto& pair : templatesList)
    {
        const auto& entityTemplate = pair.second;
        if(!entityTemplate.isAbstract())
        {
            //If it's a player, add it to the player templates list
            if(entityTemplate.isPlayer())
            {
                m_playerTemplatesNames.push_back(pair.first);
                m_playerTemplateComboBox->AppendItem(entityTemplate.getFriendlyName());
            }
            else //If not, add it to the general template list
            {
                m_templatesNames.push_back(pair.first);
                m_templatesListBox->AppendItem(entityTemplate.getFriendlyName());
            }
        }
    }
}

bool LevelEditorState::isMouseNotOnWidgets(sf::Vector2i mousePosition, sf::RenderTarget& target) const
{
    sf::Vector2f mouseCoords = target.mapPixelToCoords(mousePosition, m_guiView);

    return (
        !m_fileToolbar->GetAllocation().contains(mouseCoords) &&
        !m_toolsToolbar->GetAllocation().contains(mouseCoords) &&
        !m_toolsSettingsToolbar->GetAllocation().contains(mouseCoords)
    );
}

entityx::Entity LevelEditorState::getFirstEntityUnderMouse(sf::Vector2i mousePosition, sf::RenderTarget& target)
{
    entityx::ComponentHandle<components::PositionComponent> position;
    for(entityx::Entity entity : m_level.getEntityManager().entities_with_components(position))
    {
        if(isEntityUnderMouse(entity, mousePosition, target))
            return entity;
    }

    return entityx::Entity();
}

entityx::Entity LevelEditorState::getFirstEntityUnderMouse(sf::Vector2f position)
{
    entityx::ComponentHandle<components::PositionComponent> positionComponent;
    for(entityx::Entity entity : m_level.getEntityManager().entities_with_components(positionComponent))
    {
        if(isEntityUnderMouse(entity, position))
            return entity;
    }

    return entityx::Entity();
}

bool LevelEditorState::isEntityUnderMouse(entityx::Entity entity, sf::Vector2i mousePosition, sf::RenderTarget& target) const
{
    sf::Vector2f mouseAbsolutePos = target.mapPixelToCoords(mousePosition, m_levelView);

    return isEntityUnderMouse(entity, mouseAbsolutePos);
}

bool LevelEditorState::isEntityUnderMouse(entityx::Entity entity, sf::Vector2f position) const
{
    auto positionComponent = entity.component<components::PositionComponent>();
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

    entityx::ComponentHandle<components::PositionComponent> positionComponent;
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

}
