#include "State/Editor/PropertyWidget.hpp"

namespace state
{
namespace editor
{

PropertyWidget::PropertyWidget(entityx::Entity entity, const lua::EntityTemplate::Parameter& parameter) :
    m_entity(entity),
    m_parameter(parameter)
{

}

PropertyWidget::~PropertyWidget()
{

}

}
}
