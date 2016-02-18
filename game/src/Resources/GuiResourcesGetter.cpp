#include "Resources/GuiResourcesGetter.hpp"

namespace resources
{

std::shared_ptr<GuiResourcesGetter> GuiResourcesGetter::create(resources::FontsManager& fontsManager)
{
    return std::shared_ptr<GuiResourcesGetter>(new GuiResourcesGetter(fontsManager));
}

std::shared_ptr<sf::Font> GuiResourcesGetter::loadFont(const std::string& name)
{
    return m_fontsManager.requestResource(name);
}

GuiResourcesGetter::GuiResourcesGetter(resources::FontsManager& fontsManager) :
    m_fontsManager(fontsManager)
{

}

}
