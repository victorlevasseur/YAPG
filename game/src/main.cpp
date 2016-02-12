#include <iostream>

#include "Lua/LuaState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "State/State.hpp"
#include "State/StateEngine.hpp"
#include "State/LevelState.hpp"
#include "Window/WindowManager.hpp"

int main(int argc, char** argv)
{
    //State manager
    state::StateEngine stateEngine;

    //Settings manager
    settings::SettingsManager settingsManager("config.xml");

    //Texture manager
    resources::TexturesManager texturesManager("assets");

    //Window manager
    window::WindowManager windowManager(stateEngine, "Yet Another Platformer Game");

    stateEngine.stopAndStartState
    <state::LevelState, std::string, resources::TexturesManager&, settings::SettingsManager&>(
        "level.lua", texturesManager, settingsManager
    );

    windowManager.run();

    return 0;
}
