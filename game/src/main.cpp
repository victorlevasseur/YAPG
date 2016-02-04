#include <iostream>

#include "Lua/LuaState.hpp"
#include "Resources/ResourcesManager.hpp"
#include "State/State.hpp"
#include "State/StateEngine.hpp"
#include "State/LevelState.hpp"
#include "Window/WindowManager.hpp"

int main(int argc, char** argv)
{
    state::StateEngine stateEngine;
    resources::TexturesManager texturesManager("assets");
    window::WindowManager windowManager(stateEngine, "Yet Another Platformer Game");

    stateEngine.stopAndStartState<state::LevelState, std::string, resources::TexturesManager&>("level.lua", texturesManager);

    windowManager.run();

    return 0;
}
