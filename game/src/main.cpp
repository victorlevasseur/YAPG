#include <iostream>

#include "Lua/LuaState.hpp"
#include "State/State.hpp"
#include "State/StateEngine.hpp"
#include "State/LevelState.hpp"
#include "Window/WindowManager.hpp"

int main(int argc, char** argv)
{
    state::StateEngine stateEngine;
    window::WindowManager windowManager(stateEngine, "Yet Another Platformer Game");

    stateEngine.stopAndStartState<state::LevelState, std::string>("level.lua");

    windowManager.run();

    return 0;
}
