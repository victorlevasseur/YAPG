#include "State/LevelEditorState.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/RadioButtonGroup.hpp>
#include <SFGUI/Separator.hpp>

#include "Components/PositionComponent.hpp"
#include "Components/TemplateComponent.hpp"
#include "Lua/EntityHandle.hpp"
#include "Lua/EntityTemplate.hpp"
#include "NativeGui/FileDialog.hpp"
#include "State/MainMenuState.hpp"
#include "State/StateEngine.hpp"
#include "State/Editor/EntryPropertyWidget.hpp"
#include "Systems/RenderSystem.hpp"

namespace state
{

LevelEditorState::LevelEditorState(StateEngine& stateEngine, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    State(stateEngine),
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
    m_mouseOffsetToSelected()
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
            isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target) &&
            m_templatesListBox->GetSelectedItemsCount() > 0)
        {
            sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_levelView);

            //Insert the new entity here
            const lua::EntityTemplate& entityTemplate = m_luaState.getTemplate(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()]);

            entityx::Entity newEntity = m_level.createNewEntity(m_templatesNames[m_templatesListBox->GetSelectedItemIndex()], true);

            try
            {
                newEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("x", mousePosition.x);
                newEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("y", mousePosition.y);
            }
            catch(std::exception& e)
            {
                std::cout << "[Editor/Warning] The template \"" << entityTemplate.getName() << "\" can't be inserted because it doesn't have numerical X and Y parameters !" << std::endl;
                newEntity.destroy();
            }
        }
    }
    else if(getEditionMode() == EditionMode::Modify)
    {
        sf::Vector2f mousePosition = target.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_levelView);

        if(event.type == sf::Event::MouseButtonPressed && isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target))
        {
            m_selectedEntity = getFirstEntityUnderMouse(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target);
            m_propertiesManager->setCurrentEntity(m_selectedEntity);

            if(m_selectedEntity)
            {
                sf::Vector2f entityPos;
                entityPos.x = boost::any_cast<float>(m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.getParameter("x"));
                entityPos.y = boost::any_cast<float>(m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.getParameter("y"));

                m_mouseOffsetToSelected = mousePosition - entityPos;
            }

            //TODO: Update properties
        }
        else if(event.type == sf::Event::MouseButtonReleased && isMouseNotOnWidgets(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), target))
        {
            if(m_selectedEntity)
            {
                m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("x", event.mouseButton.x - m_mouseOffsetToSelected.x);
                m_selectedEntity.component<components::TemplateComponent>()->parametersHelper.setParameter("y", event.mouseButton.y - m_mouseOffsetToSelected.y);
            }
        }
    }
}

void LevelEditorState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 180, 255));

    //Render the level
    target.setView(m_levelView);
    m_systemMgr->system<systems::RenderSystem>()->render(target);

    if(getEditionMode() == EditionMode::Modify)
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

    if(getEditionMode() == EditionMode::View)
    {
        //If no widgets have focus, the keys can move the view
        sf::Vector2f centerPos = m_levelView.getCenter();
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            centerPos.x -= 400.f * dt.asSeconds();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            centerPos.x += 400.f * dt.asSeconds();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            centerPos.y -= 400.f * dt.asSeconds();
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            centerPos.y += 400.f * dt.asSeconds();
        }
        m_levelView.setCenter(centerPos);
    }
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
    m_viewTool = sfg::RadioButton::Create("Change view", radioGroup);

    m_insertionTool->SetActive(true);
    toolsBox->PackEnd(m_insertionTool);
    toolsBox->PackEnd(m_modifyTool);
    toolsBox->PackEnd(m_viewTool);

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
    m_propertiesManager.reset(new editor::PropertiesManager(m_propertiesScrolled));
    m_propertiesManager->registerPropertyWidget<float, editor::EntryPropertyWidget<float>>();
    m_propertiesManager->registerPropertyWidget<int, editor::EntryPropertyWidget<int>>();
    m_propertiesManager->registerPropertyWidget<unsigned int, editor::EntryPropertyWidget<unsigned int>>();
    m_propertiesManager->registerPropertyWidget<std::string, editor::EntryPropertyWidget<std::string>>();
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
    else if(m_viewTool->IsActive())
        return EditionMode::View;
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

bool LevelEditorState::isEntityUnderMouse(entityx::Entity entity, sf::Vector2i mousePosition, sf::RenderTarget& target) const
{
    sf::Vector2f mouseAbsolutePos = target.mapPixelToCoords(mousePosition, m_levelView);

    auto positionComponent = entity.component<components::PositionComponent>();
    if(!positionComponent)
        return false;

    sf::FloatRect entityBoundingBox(positionComponent->x, positionComponent->y, positionComponent->width, positionComponent->height);

    return entityBoundingBox.contains(mouseAbsolutePos);
}

}
