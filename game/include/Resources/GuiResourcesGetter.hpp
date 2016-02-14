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

    static std::shared_ptr<GuiResourcesGetter> create(resources::FontManager& fontManager);

    virtual std::shared_ptr<sf::Font> loadFont(const std::string& name);

protected:
    GuiResourcesGetter(resources::FontManager& fontManager);

private:
    resources::FontManager& m_fontManager;
};

}

#endif
