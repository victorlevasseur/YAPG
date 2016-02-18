#ifndef YAPG_GAME_RESOURCES_RESOURCESMANAGER_H
#define YAPG_GAME_RESOURCES_RESOURCESMANAGER_H

#include <map>
#include <memory>
#include <string>

#include <SFML/Audio/SoundBuffer.hpp>
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
using FontsManager = ResourcesManager<sf::Font>;
using SoundsManager = ResourcesManager<sf::SoundBuffer>;

class AllResourcesManagers
{
public:
    AllResourcesManagers(const std::string& resourcesPath) :
        m_texturesManager(resourcesPath),
        m_fontsManager(resourcesPath),
        m_soundsManager(resourcesPath)
    {

    }

    const TexturesManager& getTextures() const { return m_texturesManager; }
    TexturesManager& getTextures() { return m_texturesManager; }

    const FontsManager& getFonts() const { return m_fontsManager; }
    FontsManager& getFonts() { return m_fontsManager; }

    const SoundsManager& getSounds() const { return m_soundsManager; }
    SoundsManager& getSounds() { return m_soundsManager; }

private:
    TexturesManager m_texturesManager;
    FontsManager m_fontsManager;
    SoundsManager m_soundsManager;
};

}

#endif
