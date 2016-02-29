namespace lua
{

template<class C>
void EntityHandle::doLoadAttributeFromXml(const std::string& attributeName, const tinyxml2::XMLElement* xmlElement, const level::SerializedEntityGetter& serializedEntityGetter)
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        metadata.getAttribute(attributeName).loadFromXml(m_entity.component<C>().get(), xmlElement, serializedEntityGetter);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
    }
}

template<class C>
void EntityHandle::declareComponent(const std::string& componentName)
{
    //Create a ComponentAttributesCallbacks instance containing all the needed callbacks
    attributesCallbacks.emplace(
        componentName,
        ComponentAttributesCallbacks{
            &EntityHandle::doLoadAttributeFromXml<C>,
            &EntityHandle::doGetAttributeAsAny<C>,
            &EntityHandle::doSetAttributeAsAny<C>,
            &EntityHandle::doGetAttributeAsLuaTable<C>,
            &EntityHandle::doSetAttributeAsLuaTable<C>
        }
    );

    std::cout << "[Lua/Note] Registered component \"" << componentName << "\"." << std::endl;
}

template<class C>
boost::any EntityHandle::doGetAttributeAsAny(const std::string& attributeName) const
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        return metadata.getAttribute(attributeName).getAsAny(m_entity.component<const C>().get());
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
        return "";
    }
}

template<class C>
void EntityHandle::doSetAttributeAsAny(const std::string& attributeName, const boost::any& value)
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        metadata.getAttribute(attributeName).setAsAny(m_entity.component<C>().get(), value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
    }
}

template<class C>
void EntityHandle::doGetAttributeAsLuaTable(const std::string& attributeName, sol::table result) const
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        metadata.getAttribute(attributeName).getAsLuaTable(m_entity.component<const C>().get(), result);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
    }
}

template<class C>
void EntityHandle::doSetAttributeAsLuaTable(const std::string& attributeName, sol::table value)
{
    if(m_entity.has_component<C>())
    {
        auto& metadata = dynamic_cast<meta::ClassMetadata<C>&>(
            meta::MetadataStore::getMetadata<C>()
        );
        metadata.getAttribute(attributeName).setAsLuaTable(m_entity.component<C>().get(), value);
    }
    else
    {
        std::cout << "[Lua/Warning] Trying to access a component from an entity that doesn't have it !" << std::endl;
    }
}

}
