#include "State/LevelEditorState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFGUI/Box.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/RadioButtonGroup.hpp>
#include <SFGUI/Separator.hpp>

#include "State/MainMenuState.hpp"
#include "State/StateEngine.hpp"
#include "Systems/RenderSystem.hpp"

namespace state
{

LevelEditorState::LevelEditorState(StateEngine& stateEngine, std::string path, resources::AllResourcesManagers& resourcesManager, settings::SettingsManager& settingsManager, sfg::SFGUI& sfgui, sfg::Desktop& desktop) :
    State(stateEngine),
    m_resourcesManager(resourcesManager),
    m_settingsManager(settingsManager),
    m_luaState(),
    m_guiView(sf::FloatRect(0.f, 0.f, 1024.f, 768.f)),
    m_levelView(m_guiView),
    m_sfgui(sfgui),
    m_desktop(desktop),
    m_fileToolbar(),
    m_toolsToolbar(),
    m_toolsSettingsToolbar(),
    m_templatesScrolled(),
    m_templatesListBox(),
    m_templatesListButtons(),
    m_selectedTemplateButton(nullptr),
    m_propertiesScrolled(),
    m_level(path, m_luaState),
    m_systemMgr(nullptr)
{
    initSystemManager();
    initGUI();
    updateTemplatesList();
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
    m_templatesScrolled->Show(false);
    m_propertiesScrolled->Show(false);
}

void LevelEditorState::onUnpause()
{
    m_fileToolbar->Show(true);
    m_toolsToolbar->Show(true);
    m_toolsSettingsToolbar->Show(true);
    m_templatesScrolled->Show(true);
}

void LevelEditorState::processEvent(sf::Event event, sf::RenderTarget &target)
{
    m_desktop.HandleEvent(event);
}

void LevelEditorState::render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 180, 255));
    target.setView(m_levelView);
    m_systemMgr->system<systems::RenderSystem>()->render(target);
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
        auto fileBox = sfg::Box::Create();
        fileBox->SetSpacing(5.f);
        m_fileToolbar->Add(fileBox);

        auto newButton = sfg::Button::Create("New level");
        fileBox->PackEnd(newButton);

        auto openButton = sfg::Button::Create("Open level...");
        fileBox->PackEnd(openButton);

        auto saveButton = sfg::Button::Create("Save");
        fileBox->PackEnd(saveButton);

        auto saveAsButton = sfg::Button::Create("Save as...");
        fileBox->PackEnd(saveAsButton);

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

    m_templatesScrolled = sfg::ScrolledWindow::Create();
    toolsSettingsBox->PackEnd(m_templatesScrolled);

    m_templatesListBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
    m_templatesScrolled->AddWithViewport(m_templatesListBox);

    m_propertiesScrolled = sfg::ScrolledWindow::Create();
    m_propertiesScrolled->Show(false);
    toolsSettingsBox->PackEnd(m_propertiesScrolled);

    //END OF TOOLS TOOLBAR
    auto enableCorrectToolSettings = [&](std::weak_ptr<sfg::ScrolledWindow> toEnable, sf::String title)
    {
        m_templatesScrolled->Show(false);
        m_propertiesScrolled->Show(false);

        m_toolsSettingsToolbar->SetTitle(title);

        toEnable.lock()->Show(true);
    };

    m_insertionTool->GetSignal(sfg::ToggleButton::OnToggle).Connect(
        std::bind(enableCorrectToolSettings, m_templatesScrolled, "Templates")
    );

    m_modifyTool->GetSignal(sfg::ToggleButton::OnToggle).Connect(
        std::bind(enableCorrectToolSettings, m_propertiesScrolled, "Properties")
    );
}

void LevelEditorState::initSystemManager()
{
    m_systemMgr.reset(new entityx::SystemManager(m_level.getEntityManager(), m_level.getEventManager()));

    m_systemMgr->add<systems::RenderSystem>(m_resourcesManager.getTextures());

    m_systemMgr->configure();
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
    m_templatesListButtons.clear();
    m_templatesListBox->RemoveAll();

    auto& templatesList = m_luaState.getTemplates();
    for(auto& pair : templatesList)
    {
        const auto& entityTemplate = pair.second;
        auto templateButton = sfg::ToggleButton::Create(entityTemplate.getFriendlyName());
        templateButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind([&](std::weak_ptr<sfg::ToggleButton> _templateButton)
        {
            if(_templateButton.lock()->IsActive())
                selectTemplate(_templateButton.lock());
        }, std::weak_ptr<sfg::ToggleButton>(templateButton)));

        m_templatesListBox->PackEnd(templateButton);
        m_templatesListButtons.emplace(templateButton, entityTemplate);
    }
}

void LevelEditorState::selectTemplate(sfg::ToggleButton::Ptr entityTemplateButton)
{
    //Untoggle all buttons
    for(auto& pair : m_templatesListButtons)
    {
        if(pair.first != entityTemplateButton)
            pair.first->SetActive(false);
        else if(!entityTemplateButton->IsActive())
            pair.first->SetActive(true);
    }

    m_selectedTemplateButton = entityTemplateButton;
}

}
