#include <iostream>

#include <boost/filesystem.hpp>

#ifdef OS_LINUX
#include <gtk/gtk.h>
#endif

#include "Lua/LuaState.hpp"
#include "Menu/MainMenuState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"
#include "State/StateEngine.hpp"
#include "Template/PackagesManager.hpp"
#include "Tools/PathTools.hpp"
#include "Window/WindowManager.hpp"

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{

#ifdef OS_LINUX
    //Init GTK
    gtk_init(&argc, &argv);
#endif

    //State manager
    yapg::StateEngine stateEngine;

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
    yapg::SettingsManager settingsManager("config.xml");

    //Texture manager and font manager
    yapg::AllResourcesManagers resManager("");

    //Window manager
    yapg::WindowManager windowManager(stateEngine, "Yet Another Platformer Game");

    //Package manager
    yapg::PackagesManager::init({fs::absolute(".yapg/packages", yapg::getHomePath()), fs::path("packages")}, fs::absolute(".yapg/packages", yapg::getHomePath()));

    stateEngine.stopAndStartState
    <yapg::MainMenuState, yapg::AllResourcesManagers&, yapg::SettingsManager&>(
        resManager, settingsManager
    );

    windowManager.run();

    return 0;
}
