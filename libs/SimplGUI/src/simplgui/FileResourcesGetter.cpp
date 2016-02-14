#include "simplgui/FileResourcesGetter.h"

#include <iostream>

namespace simplgui
{

std::shared_ptr<FileResourcesGetter> FileResourcesGetter::create(const std::string &directoryPrefix)
{
    return std::shared_ptr<FileResourcesGetter>(new FileResourcesGetter(directoryPrefix));
}

std::shared_ptr<sf::Font> FileResourcesGetter::loadFont(const std::string &name)
{
    std::shared_ptr<sf::Font> loadedFont(new sf::Font());
    if(loadedFont->loadFromFile(directoryPrefix + name))
    {
        std::cout << "[STATUS] simplgui::FileResourcesGetter: Successfully loaded " << name << " font." << std::endl;
        return loadedFont;
    }
    else
    {
        std::cout << "[ERROR] simplgui::FileResourcesGetter: Can't find " << name << " font." << std::endl;
        return std::shared_ptr<sf::Font>(nullptr);
    }
}

}

