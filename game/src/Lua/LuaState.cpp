#include "Lua/LuaState.hpp"

#include <iostream>

#include <boost/filesystem.hpp>

#include "Components/PositionComponent.hpp"
#include "Components/CustomBehaviorComponent.hpp"
#include "Lua/EntityHandle.hpp"

namespace fs = boost::filesystem;

namespace game
{
namespace lua
{

LuaState::LuaState() :
    m_luaState()
{
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

    //Declare the metadatas of some basic types
    MetadataStore::registerLuaAssignableType<int>();
    MetadataStore::registerLuaAssignableType<unsigned int>();
    MetadataStore::registerLuaAssignableType<float>();
    MetadataStore::registerLuaAssignableType<double>();
    MetadataStore::registerLuaAssignableType<sol::function>();

    //Declare main C++ classes and declare their metadatas
    EntityHandle::registerClass(*this);
    components::PositionComponent::registerComponent(*this);
    components::CustomBehaviorComponent::registerComponent(*this);

    //Load yapg core libraries
    m_luaState.open_file("scripts/core/array_tools.lua");
    m_luaState.open_file("scripts/game/template_tools.lua");
    std::cout << "Loaded core and game lua libraries." << std::endl;

    //Load templates
    loadTemplates(std::string("templates"));

    //TODO
}

sol::state& LuaState::getState()
{
    return m_luaState;
}

const sol::state& LuaState::getState() const
{
    return m_luaState;
}

int LuaState::getTableSize(const std::string& tableName)
{
    std::string command("temp = array_tools.table_size(" + tableName + ")");
    m_luaState.script(command);

    return m_luaState.get<int>("temp");
}

std::vector<std::string> LuaState::getTableKeys(const std::string& tableName)
{
    std::string command("temp = array_tools.get_keys(" + tableName + ")");
    m_luaState.script(command);

    std::string s = m_luaState.get<std::string>("temp");

    char c = '|';
    std::string buff{""};
	std::vector<std::string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);

	return v;
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
                    m_luaState.open_file(filePath);
                    std::cout << "Loaded " << e.path() << std::endl;
                }
            }
        }
        else
        {
            std::cout << "Can't find templates in " << path << " ! (not exists or not a directory)" << std::endl;
        }
    }
    catch(const fs::filesystem_error &exc)
    {
        std::cout << exc.what() << std::endl;
    }
}

}
}
