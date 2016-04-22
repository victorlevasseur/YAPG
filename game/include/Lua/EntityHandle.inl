namespace lua
{

template<class C>
components::Component* EntityHandle::doGetComponentPtr()
{
    if(m_entity.has_component<C>())
    {
        return m_entity.component<C>().get();
    }

    return nullptr;
}

template<class C>
const components::Component* EntityHandle::doGetComponentPtrConst() const
{
    if(m_entity.has_component<C>())
    {
        return m_entity.component<const C>().get();
    }

    return nullptr;
}

template<class C>
bool EntityHandle::doHasComponent() const
{
    return m_entity.has_component<C>();
}

template<class C>
void EntityHandle::declareComponent(const std::string& componentName)
{
    //Create a ComponentAttributesCallbacks instance containing all the needed callbacks
    componentsTypeIndex.emplace(componentName, typeid(C));
    componentsGetters.emplace(componentName, &EntityHandle::doGetComponentPtr<C>);
    componentsGettersConst.emplace(componentName, &EntityHandle::doGetComponentPtrConst<C>);
    componentsCheckers.emplace(componentName, &EntityHandle::doHasComponent<C>);

    std::cout << "[Lua/Note] Registered component \"" << componentName << "\"." << std::endl;
}

}
