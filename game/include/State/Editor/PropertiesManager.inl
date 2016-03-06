namespace state
{
namespace editor
{

template<typename T, typename U>
void PropertiesManager::registerPropertyWidget()
{
    m_registeredWidgets.emplace(
        typeid(T),
        [](entityx::Entity entity, const lua::EntityTemplate::Parameter& parameter)
        {
            return std::make_unique<U>(entity, parameter);
        }
    );
}

}
}
