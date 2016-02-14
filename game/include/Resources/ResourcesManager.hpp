#ifndef YAPG_GAME_RESOURCES_RESOURCESMANAGER_H
#define YAPG_GAME_RESOURCES_RESOURCESMANAGER_H

#include <map>
#include <memory>
#include <string>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace resources
{

/**
 * Class allowing game element to request resources to be loaded with its filename.
 * T is the resource class, must be default constructible, have a
 * loadFromFile(const std::string&) method.
 *
 * When requesting a resource, the class loads the resources if not done yet and
 * returns a std::shared_ptr to the resources. The class tracks the loaded resource
 * using a weak_ptr, so that if the resource is not more used it's unloaded.
 */
template<class T>
class ResourcesManager
{
public:
    ResourcesManager(const std::string& resourcesPath) :
        m_resourcesPath(resourcesPath),
        m_resources()
    {

    }

    std::shared_ptr<T> requestResource(const std::string& path) const
    {
        if(m_resources[path].expired())
        {
            auto resource = std::make_shared<T>();
            resource->loadFromFile(m_resourcesPath + "/" + path);
            m_resources[path] = resource;
            return resource;
        }
        else
        {
            return m_resources[path].lock();
        }
    }

private:
    std::string m_resourcesPath;
    mutable std::map<std::string, std::weak_ptr<T>> m_resources;
};

using TexturesManager = ResourcesManager<sf::Texture>;
using FontManager = ResourcesManager<sf::Font>;

}

#endif
