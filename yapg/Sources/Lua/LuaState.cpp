#include "Lua/LuaState.hpp"

#include <iostream>

#include <boost/any.hpp>
#include <boost/filesystem.hpp>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "Collision/CollidableComponent.hpp"
#include "Collision/ColliderComponent.hpp"
#include "Collision/Polygon.hpp"
#include "Collision/PolygonCallback.hpp"
#include "Common/PositionComponent.hpp"
#include "Health/HealthComponent.hpp"
#include "Lua/CustomBehaviorComponent.hpp"
#include "Lua/CustomDataComponent.hpp"
#include "Level/Level.hpp"
#include "Level/LevelState.hpp"
#include "Lua/EntityHandle.hpp"
#include "Meta/Metadata.hpp"
#include "Platformer/PlatformerHitboxComponent.hpp"
#include "Platformer/PlatformComponent.hpp"
#include "Platformer/PlatformerComponent.hpp"
#include "Player/PlayerComponent.hpp"
#include "Rendering/Animation/Animation.hpp"
#include "Rendering/Animation/Frame.hpp"
#include "Rendering/RenderComponent.hpp"
#include "Settings/tinyxml2.h"

namespace fs = boost::filesystem;
namespace tx2 = tinyxml2;

namespace yapg
{

LuaState::LuaState(bool loadAllTemplates) :
    m_luaState(),
    m_templatesPackages()
{
    std::cout << "[Lua/Note] Starting lua state initialization process..." << std::endl;
    //Loading lua libraries
    m_luaState.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::coroutine,
        sol::lib::string,
        sol::lib::table,
        sol::lib::math,
        sol::lib::bit32,
        sol::lib::io,
        sol::lib::os
    );

    m_luaState.new_usertype<boost::any>("boost_any"); //boost::any declaration to lua

    //Declare the metadatas of some basic types
    meta::MetadataStore::registerType<int>()
        .setXmlLoadFunction([](int* value, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->QueryIntText(value);
        })
        .setXmlSaveFunction([](const int* value, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->SetText(*value);
        });
    declareAnyConvertibleType<int>("int");

    meta::MetadataStore::registerType<unsigned int>()
        .setXmlLoadFunction([](unsigned int* value, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            int intValue = static_cast<int>(*value);
            xmlElement->QueryIntText(&intValue);
            *value = static_cast<unsigned int>(intValue);
        })
        .setXmlSaveFunction([](const unsigned int* value, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->SetText(static_cast<int>(*value));
        });
    declareAnyConvertibleType<unsigned int>("unsigned_int");

    meta::MetadataStore::registerType<bool>()
        .setXmlLoadFunction([](bool* value, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            const char* stringValue = xmlElement->GetText();
            if(stringValue)
                *value = strcmp(stringValue, "true");
        })
        .setXmlSaveFunction([](const bool* value, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->SetText(*value);
        });
    declareAnyConvertibleType<bool>("bool");

    meta::MetadataStore::registerType<float>()
        .setXmlLoadFunction([](float* value, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->QueryFloatText(value);
        })
        .setXmlSaveFunction([](const float* value, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->SetText(*value);
        });
    declareAnyConvertibleType<float>("float");

    meta::MetadataStore::registerType<double>()
        .setXmlLoadFunction([](double* value, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->QueryDoubleText(value);
        })
        .setXmlSaveFunction([](const double* value, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->SetText(*value);
        });
    declareAnyConvertibleType<double>("double");

    meta::MetadataStore::registerType<std::string>()
        .setXmlLoadFunction([](std::string* value, const tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            const char* stringValue = xmlElement->GetText();
            if(stringValue)
                *value = std::string(stringValue);
        })
        .setXmlSaveFunction([](const std::string* value, tinyxml2::XMLElement* xmlElement, const SerializedEntityGetter& entityGetter)
        {
            xmlElement->SetText(value->c_str());
        });
    declareAnyConvertibleType<std::string>("string");

    meta::MetadataStore::registerType<sol::function>();

    meta::MetadataStore::registerClass<sf::Vector2f>()
        .declareAttribute("x", &sf::Vector2f::x)
        .declareAttribute("y", &sf::Vector2f::y);
    getState().new_usertype<sf::Vector2f>("sf_vector_2f",
        "x", &sf::Vector2f::x,
        "y", &sf::Vector2f::y);

    meta::MetadataStore::registerClass<sf::IntRect>()
        .declareAttribute("left", &sf::IntRect::left)
        .declareAttribute("top", &sf::IntRect::top)
        .declareAttribute("width", &sf::IntRect::width)
        .declareAttribute("height", &sf::IntRect::height);
    getState().new_usertype<sf::FloatRect>("sf_float_rect",
        "left", &sf::FloatRect::left,
        "top", &sf::FloatRect::top,
        "width", &sf::FloatRect::width,
        "height", &sf::FloatRect::height);

    std::cout << "[Lua/Note] Primitive types registered." << std::endl;

    //Declare class metadatas
    Animation::registerClass();
    Frame::registerClass();
    Polygon::registerClass(*this);
    PolygonCallback::registerClass(*this);
    std::cout << "[Lua/Note] Classes registered." << std::endl;

    //Declare main C++ classes and declare their metadatas
    EntityHandle::registerClass(*this);
    Level::registerClass(*this);
    LevelState::registerClass(*this);

    Component::registerComponent(*this);
    CollidableComponent::registerComponent(*this);
    ColliderComponent::registerComponent(*this);
    CustomBehaviorComponent::registerComponent(*this);
    CustomDataComponent::registerComponent(*this);
    HealthComponent::registerComponent(*this);
    PlatformerHitboxComponent::registerComponent(*this);
    PlatformComponent::registerComponent(*this);
    PlatformerComponent::registerComponent(*this);
    PlayerComponent::registerComponent(*this);
    PositionComponent::registerComponent(*this);
    RenderComponent::registerComponent(*this);
    std::cout << "[Lua/Note] All components registered." << std::endl;

    //Load yapg core libraries
    m_luaState.script_file("scripts/core/array_tools.lua");
    m_luaState.script_file("scripts/game/position_tools.lua");
    std::cout << "[Lua/Note] Scripting tools loaded." << std::endl;

    //Load templates
    if(loadAllTemplates)
    {
        LuaState::loadAllTemplates();
    }

    std::cout << "[Lua/Note] --> Lua state initialization completed." << std::endl;
}

sol::state& LuaState::getState()
{
    return m_luaState;
}

const sol::state& LuaState::getState() const
{
    return m_luaState;
}

namespace
{
    std::pair<std::string, std::string> decomposeTemplateName(const std::string& fullname)
    {
        std::size_t lastPointPosition = fullname.rfind('.');
        if(lastPointPosition == std::string::npos || lastPointPosition == 0 || lastPointPosition == fullname.size() - 1)
            throw std::runtime_error("[Template/Error] Invalid template name requested: \"" + fullname + "\" !");

        std::string packageName = fullname.substr(0, lastPointPosition);
        std::string templateName = fullname.substr(lastPointPosition + 1, std::string::npos);

        return std::make_pair(packageName, templateName);
    }
}

const EntityTemplate& LuaState::getTemplate(const std::string& name) const
{
    return m_templatesPackages.at(decomposeTemplateName(name).first).templates.at(decomposeTemplateName(name).second);
}

bool LuaState::hasTemplate(const std::string& name) const
{
    return m_templatesPackages.count(decomposeTemplateName(name).first) != 0
        && m_templatesPackages.at(decomposeTemplateName(name).first).templates.count(decomposeTemplateName(name).second) != 0;
}

void LuaState::loadAllTemplates()
{
    loadPackages(std::string("packages"));
    std::cout << "[Lua/Note] Applying block inheritance..." << std::endl;
    for(auto packageIt = m_templatesPackages.begin(); packageIt != m_templatesPackages.end(); ++packageIt)
    {
        for(auto it = packageIt->second.templates.begin(); it != packageIt->second.templates.end(); )
        {
            auto currentIt = it++;

            try
            {
                currentIt->second.applyInheritance(*this);
            }
            catch(const std::out_of_range& e)
            {
                //The template didn't find its base template, remove it from the templates
                std::cout << "[Lua/Warning] Can't find the base template of \"" << currentIt->second.getName() << "\", the template will be ignored !" << std::endl;
                packageIt->second.templates.erase(currentIt);
            }
        }
    }
    std::cout << "[Lua/Note] Entities templates loaded." << std::endl;
}

void LuaState::unloadAllTemplates()
{
    m_templatesPackages.clear();
}

sol::table LuaState::mergeTables(sol::table first, sol::table second)
{
    sol::function deepCopyFunc = m_luaState.get<sol::table>("array_tools").get<sol::function>("deep_copy");
    sol::function mergeTablesFunc = m_luaState.get<sol::table>("array_tools").get<sol::function>("merge_tables");

    sol::table result = deepCopyFunc.call<sol::table>(first);
    result = mergeTablesFunc.call<sol::table>(result, second);

    return result;
}

void LuaState::loadPackages(const std::string& path)
{
    loadPackages(fs::path(path));
}

void LuaState::loadPackages(const fs::path& path)
{
    try
    {
        if(fs::exists(path) && fs::is_directory(path))
        {
            //Iterate over each packages
            for(fs::directory_entry& e : fs::directory_iterator(path))
            {
                if(fs::is_directory(e.path()))
                {
                    std::string packagePath = e.path().string();

                    //Test if it's a package folder
                    tx2::XMLDocument packageMetadata;
                    std::string packageInfoFilePath = packagePath + "/PackageInfo.xml";
                    if(packageMetadata.LoadFile(packageInfoFilePath.data()) != tx2::XML_NO_ERROR)
                    {
                        continue;
                    }

                    tx2::XMLElement* rootElem = packageMetadata.RootElement();
                    if(strcmp(rootElem->Name(), "yapg_package") != 0)
                    {
                        continue;
                    }

                    tx2::XMLElement* packageFriendlyElem = rootElem->FirstChildElement("friendly_name");
                    if(packageFriendlyElem->GetText() == nullptr)
                    {
                        continue;
                    }

                    TemplatesPackage newPackage;
                    newPackage.packageName = fs::relative(e.path(), path).string();
                    newPackage.packageFriendlyName = std::string(packageFriendlyElem->GetText());

                    //Load the templates of the packages
                    for(fs::directory_entry& e2 : fs::directory_iterator(fs::canonical(e.path())))
                    {
                        if(fs::is_regular_file(e2.path()) && e2.path().extension().string() == ".lua")
                        {
                            std::string filePath = fs::canonical(e2.path()).string();
                            try
                            {
                                m_luaState.script_file(filePath);

                                newPackage.templates.emplace(
                                    m_luaState.get<sol::table>("entity_template").get<std::string>("name"),
                                    EntityTemplate(m_luaState.get<sol::table>("entity_template"), newPackage.packageName)
                                );
                                std::cout << "[Lua/Note] Loaded template from " << e2.path() << "." << std::endl;
                            }
                            catch(const sol::error& e)
                            {
                                std::cout << "[Lua/Warning] Can't load \"" << filePath << "\" because it contains an error :" << std::endl;
                                std::cout << e.what() << std::endl;
                            }
                        }
                    }

                    m_templatesPackages[newPackage.packageName] = std::move(newPackage);
                }
            }
        }
        else
        {
            std::cout << "[Lua/Warning] The package folder doesn't exist !" << std::endl;
        }
    }
    catch(const fs::filesystem_error &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}

}
