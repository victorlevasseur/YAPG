namespace yapg
{

template<typename T, typename U>
void PropertiesManager::registerPropertyWidget()
{
    m_registeredWidgets.emplace(
        typeid(T),
        [](entityx::Entity entity, const EntityTemplate::Parameter& parameter)
        {
            return std::make_unique<U>(entity, parameter);
        }
    );
}

}
