#include <iostream>

#include <boost/filesystem.hpp>

#include "Lua/LuaState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"
#include "State/StateEngine.hpp"
#include "State/MainMenuState.hpp"
#include "Window/WindowManager.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    //State manager
    state::StateEngine stateEngine;

    //Settings manager

    //If the settings file doesn't exists, use the default_config.xml file to
    //create it
    if(!fs::exists(fs::path("config.xml")))
    {
        std::cout << "First time launch, copying default_config.xml as config.xml...";
        fs::copy_file(fs::path("default_config.xml"), fs::path("config.xml"));
        std::cout << " [OK]" << std::endl;
    }

    //Load the settings
    settings::SettingsManager settingsManager("config.xml");

    //Texture manager and font manager
    resources::AllResourcesManagers resManager("assets");

    //Window manager
    window::WindowManager windowManager(stateEngine, "Yet Another Platformer Game");

    stateEngine.stopAndStartState
    <state::MainMenuState, resources::AllResourcesManagers&, settings::SettingsManager&>(
        resManager, settingsManager
    );

    windowManager.run();

    return 0;
}
