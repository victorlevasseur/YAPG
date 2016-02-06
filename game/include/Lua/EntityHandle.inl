namespace lua
{

template<class C>
void EntityHandle::declareComponent(const std::string& componentName)
{
    //Create a ComponentAttributesCallbacks instance containing all the needed callbacks
    attributesCallbacks.emplace(
        componentName,
        ComponentAttributesCallbacks{
            &EntityHandle::doGetAttributeAsString<C>,
            &EntityHandle::doSetAttributeAsString<C>
        }
    );

    std::cout << "[Lua/Note] Registered component \"" << componentName << "\"." << std::endl;
}

template<class C>
std::string EntityHandle::doGetAttributeAsString(const std::string& attributeName) const
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        return metadata.getAttribute(attributeName).getAsString(m_entity.component<const C>().get());
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
        return "";
    }
}

template<class C>
void EntityHandle::doSetAttributeAsString(const std::string& attributeName, const std::string& value)
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        metadata.getAttribute(attributeName).setAsString(m_entity.component<C>().get(), value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
    }
}

}
