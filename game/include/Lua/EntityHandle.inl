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
            &EntityHandle::doSetAttributeAsString<C>,
            &EntityHandle::doGetAttributeAsBool<C>,
            &EntityHandle::doSetAttributeAsBool<C>,
            &EntityHandle::doGetAttributeAsDouble<C>,
            &EntityHandle::doSetAttributeAsDouble<C>
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

template<class C>
bool EntityHandle::doGetAttributeAsBool(const std::string& attributeName) const
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        return metadata.getAttribute(attributeName).getAsBool(m_entity.component<const C>().get());
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
        return false;
    }
}

template<class C>
void EntityHandle::doSetAttributeAsBool(const std::string& attributeName, bool value)
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        metadata.getAttribute(attributeName).setAsBool(m_entity.component<C>().get(), value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
    }
}

template<class C>
double EntityHandle::doGetAttributeAsDouble(const std::string& attributeName) const
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        return metadata.getAttribute(attributeName).getAsDouble(m_entity.component<const C>().get());
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
        return 0.0;
    }
}

template<class C>
void EntityHandle::doSetAttributeAsDouble(const std::string& attributeName, double value)
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        metadata.getAttribute(attributeName).setAsDouble(m_entity.component<C>().get(), value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
    }
}

}
