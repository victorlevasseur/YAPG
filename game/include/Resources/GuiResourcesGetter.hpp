#ifndef YAPG_GAME_RESOURCES_GUIRESOURCESGETTER_H
#define YAPG_GAME_RESOURCES_GUIRESOURCESGETTER_H

#include <memory>
#include <string>

#include "simplgui/ResourcesGetter.h"

#include "Resources/ResourcesManager.hpp"

namespace resources
{

class GuiResourcesGetter : public simplgui::ResourcesGetter
{
public:
    using Ptr = std::shared_ptr<GuiResourcesGetter>;

    static std::shared_ptr<GuiResourcesGetter> create(resources::FontsManager& fontsManager);

    virtual std::shared_ptr<sf::Font> loadFont(const std::string& name);

protected:
    GuiResourcesGetter(resources::FontsManager& fontsManager);

private:
    resources::FontsManager& m_fontsManager;
};

}

#endif
