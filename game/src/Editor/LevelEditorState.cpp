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
    m_level(m_luaState, level::Level::LevelMode::EditMode),
    m_filepath(),
    m_systemMgr(nullptr),
    m_selectedEntity(),
    m_mouseOffsetToSelected(),
    m_dragging(false),
    m_draggingView(false),
    m_mousePosBeforeDrag()
{
    initSystemManager();
    initGUI();
    updateTemplatesList();

    newLevel();
}

void LevelEditorState::onStop()
{
    m_desktop.Remove(m_fileToolbar);
    m_desktop.Remove(m_toolsToolbar);
    m_desktop.Remove(m_toolsSettingsToolbar);
}

void LevelEditorState::onPause()
{
    m_fileToolbar->Show(false);
    m_toolsToolbar->Show(false);
    m_toolsSettingsToolbar->Show(false);
}

void LevelEditorState::onUnpause()
{
    m_fileToolbar->Show(true);
    m_toolsToolbar->Show(true);
    m_toolsSettingsToolbar->Show(true);
}

void LevelEditorState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    m_desktop.HandleEvent(event);

    if(getEditionMode() == EditionMode::Insertion)
    {
        m_selectedEntity = entityx::Entity();

        if(event.type == sf::Event::MouseButtonPressed &&
            isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target))
        {
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_levelView);

            if(m_templatesListBox->GetSelectedItemsCount() > 0 && event.mouseButton.button == sf::Mouse::Left)
            {
                //Insert the new entity here
                const lua::EntityTemplate& entityTemplate = m_luaState.getTemplate(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()]);

                entityx::Entity newEntity = m_level.createNewEntity(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()], true);

                try
                {
                    sf::Vector2f position = mousePosition;
                    if(newEntity.has_component<components::PositionComponent>())
                    {
                        auto positionComponent = newEntity.component<components::PositionComponent>();
                        position = getInsertionPosition(
                            mousePosition - sf::Vector2f(positionComponent->width / 2.f, positionComponent->height / 2.f),
                            positionComponent->width,
                            positionComponent->height
                        );
                    }
                    newEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("x", position.x);
                    newEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("y", position.y);
                }
                catch(std::exception& e)
                {
                    std::cout << "[Editor/Warning] The template \"" << entityTemplate.getName() << "\" can't be inserted because it doesn't have numerical X and Y parameters !" << std::endl;
                    newEntity.destroy();
                }
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

    if(getEditionMode() == EditionMode::Insertion)
    {
        if(m_templatesListBox->GetSelectedItemsCount() > 0)
        {
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Mouse::getPosition(dynamic_cast<sf::RenderWindow&>(target)), m_levelView);

            const lua::EntityTemplate& selectedTemplate = m_luaState.getTemplate(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()]);
            if(selectedTemplate.getComponentsTable().get<sol::object>("Position").is<sol::table>())
            {
                sol::table positionComponent = selectedTemplate.getComponentsTable().get<sol::object>("Position").as<sol::table>();

                if(positionComponent.get<sol::object>("width").is<float>() && positionComponent.get<sol::object>("height").is<float>())
                {
                    float width = positionComponent.get<sol::object>("width").as<float>();
                    float height = positionComponent.get<sol::object>("height").as<float>();

                    sf::RectangleShape ghostRect(sf::Vector2f(width, height));
                    ghostRect.setPosition(getInsertionPosition(mousePosition - sf::Vector2f(width / 2.f, height / 2.f), width, height));
                    ghostRect.setFillColor(sf::Color(0, 0, 255, 100));
                    ghostRect.setOutlineThickness(1.f);
                    ghostRect.setOutlineColor(sf::Color(0, 0, 255, 128));
                    target.draw(ghostRect);
                }
            }
        }
    }
    else if(getEditionMode() == EditionMode::Modify)
    {
        //Draw the selection box
        if(m_selectedEntity)
        {
            auto position = m_selectedEntity.component<components::PositionComponent>();
            sf::RectangleShape selectionRect(sf::Vector2f(position->width, position->height));
            selectionRect.setPosition(sf::Vector2f(position->x, position->y));
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
            getStateEngine().stopAndStartState
            <state::MainMenuState, resources::AllResourcesManagers&, settings::SettingsManager&, sfg::SFGUI&, sfg::Desktop&>(
                m_resourcesManager, m_settingsManager, m_sfgui, m_desktop
            );
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

    m_insertionTool->SetActive(true);
    toolsBox->PackEnd(m_insertionTool);
    toolsBox->PackEnd(m_modifyTool);

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
    toolsSettingsBox->PackEnd(m_templatesListBox);

    m_propertiesScrolled = sfg::ScrolledWindow::Create();
    m_propertiesScrolled->Show(false);
    toolsSettingsBox->PackEnd(m_propertiesScrolled);

    //END OF TOOLS TOOLBAR
    auto enableCorrectToolSettings = [&](std::weak_ptr<sfg::Widget> toEnable, sf::String title)
    {
        m_templatesListBox->Show(false);
        m_propertiesScrolled->Show(false);

        m_toolsSettingsToolbar->SetTitle(title);

        toEnable.lock()->Show(true);
    };

    m_insertionTool->GetSignal(sfg::ToggleButton::OnToggle).Connect(
        std::bind(enableCorrectToolSettings, m_templatesListBox, "Templates")
    );

    m_modifyTool->GetSignal(sfg::ToggleButton::OnToggle).Connect(
        std::bind(enableCorrectToolSettings, m_propertiesScrolled, "Properties")
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

    m_systemMgr->add<systems::RenderSystem>(m_resourcesManager.getTextures());

    m_systemMgr->configure();
}

void LevelEditorState::newLevel()
{
    m_level.LoadFromFile("newlevel.xml");
    m_filepath = std::string();

    m_filepathLabel->SetText("(not saved yet)");
}

void LevelEditorState::openLevel()
{
    nativegui::FileDialog fileDialog("Select a level to open...", nativegui::FileDialog::Open, { { "XML levels", {"*.xml"} } });
    if(fileDialog.run())
    {
        m_level.LoadFromFile(fileDialog.getFilename());
        m_filepath = fileDialog.getFilename();

        m_filepathLabel->SetText(m_filepath);
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

LevelEditorState::EditionMode LevelEditorState::getEditionMode() const
{
    if(m_insertionTool->IsActive())
        return EditionMode::Insertion;
    else if(m_modifyTool->IsActive())
        return EditionMode::Modify;
    return EditionMode::Unknown;
}

void LevelEditorState::updateTemplatesList()
{
    //Clear all buttons
    m_templatesListBox->Clear();
    m_templatesNames.clear();

    auto& templatesList = m_luaState.getTemplates();
    for(auto& pair : templatesList)
    {
        const auto& entityTemplate = pair.second;

        m_templatesNames.push_back(pair.first);
        m_templatesListBox->AppendItem(entityTemplate.getFriendlyName());
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
