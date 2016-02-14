#include "Resources/GuiResourcesGetter.hpp"

namespace resources
{

std::shared_ptr<GuiResourcesGetter> GuiResourcesGetter::create(resources::FontManager& fontManager)
{
    return std::shared_ptr<GuiResourcesGetter>(new GuiResourcesGetter(fontManager));
}

std::shared_ptr<sf::Font> GuiResourcesGetter::loadFont(const std::string& name)
{
    return m_fontManager.requestResource(name);
}

GuiResourcesGetter::GuiResourcesGetter(resources::FontManager& fontManager) :
    m_fontManager(fontManager)
{

}

}
