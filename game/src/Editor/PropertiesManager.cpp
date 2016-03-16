#include "Editor/PropertiesManager.hpp"

#include <boost/lexical_cast.hpp>

#include <SFGUI/Entry.hpp>
#include <SFGUI/Label.hpp>

#include "Components/TemplateComponent.hpp"
#include "Editor/EntryPropertyWidget.hpp"

namespace editor
{

PropertiesManager::PropertiesManager(sfg::ScrolledWindow::Ptr propertiesScrolled) :
    m_propertiesScrolled(propertiesScrolled),
    m_propertiesTable(sfg::Table::Create()),
    m_propertiesWidgets(),
    m_currentEntity(),
    m_registeredWidgets()
{
    m_propertiesScrolled->AddWithViewport(m_propertiesTable);
    m_propertiesTable->SetRowSpacings(5.f);
    m_propertiesTable->SetRequisition(sf::Vector2f(200.f, 0.f));
}

void PropertiesManager::setCurrentEntity(entityx::Entity currentEntity)
{
    m_currentEntity = currentEntity;

    //Clear the properties widgets
    m_propertiesTable->RemoveAll();
    m_propertiesWidgets.clear();

    if(m_currentEntity)
    {
        auto templateComponent = m_currentEntity.component<components::TemplateComponent>();

        //Add some labels to show the template infos
        m_propertiesTable->Attach(
            sfg::Label::Create("Template: "),
            sf::Rect<sf::Uint32>(0u, 0u, 1u, 1u),
            sfg::Table::FILL
        );
        m_propertiesTable->Attach(
            sfg::Label::Create(templateComponent->templateName),
            sf::Rect<sf::Uint32>(1u, 0u, 1u, 1u),
            sfg::Table::FILL|sfg::Table::EXPAND
        );

        m_propertiesTable->Attach(
            sfg::Label::Create("Instance id: "),
            sf::Rect<sf::Uint32>(0u, 1u, 1u, 1u),
            sfg::Table::FILL
        );
        m_propertiesTable->Attach(
            sfg::Label::Create(boost::lexical_cast<std::string>(m_currentEntity.id())),
            sf::Rect<sf::Uint32>(1u, 1u, 1u, 1u),
            sfg::Table::FILL
        );

        //Add the new properties widgets
        unsigned int position = 2;
        for(auto it = templateComponent->parametersHelper.parametersBegin();
            it != templateComponent->parametersHelper.parametersEnd();
            ++it)
        {
            sfg::Label::Ptr parameterLabel = sfg::Label::Create(it->second.friendlyName + ": ");
            m_propertiesTable->Attach(
                parameterLabel,
                sf::Rect<sf::Uint32>(0u, position, 1u, 1u),
                sfg::Table::FILL
            );

            //Add the corresponding property widget (if not available, put a text saying that the parameter is not editable from the editor !)
            std::type_index parameterType = templateComponent->parametersHelper.getParameterType(it->first);
            if(m_registeredWidgets.count(parameterType) > 0)
            {
                m_propertiesWidgets.emplace_back(
                    m_registeredWidgets.at(parameterType)(m_currentEntity, it->second)
                );

                m_propertiesTable->Attach(
                    m_propertiesWidgets.back()->getWidget(),
                    sf::Rect<sf::Uint32>(1u, position, 1u, 1u),
                    sfg::Table::FILL|sfg::Table::EXPAND
                );
            }
            else
            {
                m_propertiesTable->Attach(
                    sfg::Label::Create("Not supported in editor!"),
                    sf::Rect<sf::Uint32>(1u, position, 1u, 1u),
                    sfg::Table::FILL|sfg::Table::EXPAND
                );
            }


            ++position;
        }
    }
}

}
