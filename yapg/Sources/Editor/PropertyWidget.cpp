#include "Editor/PropertyWidget.hpp"

namespace yapg
{

PropertyWidget::PropertyWidget(entityx::Entity entity, const EntityTemplate::Parameter& parameter) :
    m_entity(entity),
    m_parameter(parameter)
{

}

PropertyWidget::~PropertyWidget()
{

}

}
