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

namespace fs = boost::filesystem;


namespace yapg
{

LuaState::LuaState() :
    m_luaState(),
    m_templates()
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
    loadTemplates(std::string("templates"));
    std::cout << "[Lua/Note] Applying block inheritance..." << std::endl;
    for(auto& pair : m_templates)
        pair.second.applyInheritance(*this);
    std::cout << "[Lua/Note] Entities templates loaded." << std::endl;

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

const EntityTemplate& LuaState::getTemplate(const std::string& name) const
{
    return m_templates.at(name);
}

sol::table LuaState::mergeTables(sol::table first, sol::table second)
{
    sol::function deepCopyFunc = m_luaState.get<sol::table>("array_tools").get<sol::function>("deep_copy");
    sol::function mergeTablesFunc = m_luaState.get<sol::table>("array_tools").get<sol::function>("merge_tables");

    sol::table result = deepCopyFunc.call<sol::table>(first);
    result = mergeTablesFunc.call<sol::table>(result, second);

    return result;
}

void LuaState::loadTemplates(const std::string& path)
{
    loadTemplates(fs::path(path));
}

void LuaState::loadTemplates(const fs::path& path)
{
    try
    {
        if(fs::exists(path) && fs::is_directory(path))
        {
            for(fs::directory_entry& e : fs::directory_iterator(path))
            {
                if(fs::is_directory(e.path()))
                {
                    //Do a recursive search
                    loadTemplates(e.path());
                }
                else if(fs::is_regular_file(e.path()))
                {
                    std::string filePath = fs::canonical(e.path()).string();
                    m_luaState.script_file(filePath);

                    m_templates.emplace(
                        m_luaState.get<sol::table>("entity_template").get<std::string>("name"),
                        EntityTemplate(m_luaState.get<sol::table>("entity_template"))
                    );
                    std::cout << "[Lua/Note] Loaded template from " << e.path() << "." << std::endl;
                }
            }
        }
        else
        {
            std::cout << "[Lua/Warning] Can't find templates in " << path << " ! (not exists or not a directory)" << std::endl;
        }
    }
    catch(const fs::filesystem_error &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}

}
