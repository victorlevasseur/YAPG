#ifndef SIMPLGUI_FILERESOURCESGETTER_H
#define SIMPLGUI_FILERESOURCESGETTER_H

#include <memory>
#include <SFML/Graphics/Font.hpp>
#include "simplgui/ResourcesGetter.h"

namespace simplgui
{

class FileResourcesGetter : public ResourcesGetter
{
public:
    using Ptr = std::shared_ptr<FileResourcesGetter>;

    static std::shared_ptr<FileResourcesGetter> create(const std::string &directoryPrefix = "");

    virtual std::shared_ptr<sf::Font> loadFont(const std::string &name);
    
protected:
    FileResourcesGetter(const std::string &directoryPrefix) : ResourcesGetter(), directoryPrefix(directoryPrefix) {}; 
    
private:
    std::string directoryPrefix;
};

}

#endif

