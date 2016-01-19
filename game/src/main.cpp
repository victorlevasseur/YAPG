#include <iostream>

#include "selene.h"

#include "Lua/LuaState.hpp"
#include "State/State.hpp"
#include "State/StateEngine.hpp"
#include "State/LevelState.hpp"
#include "Window/WindowManager.hpp"

namespace game
{

int main(int argc, char** argv)
{
    state::StateEngine stateEngine;
    window::WindowManager windowManager(stateEngine, "Yet Another Platformer Game");

    stateEngine.stopAndStartState<state::LevelState, std::string>("level.lua");

    windowManager.run();

    return 0;
}

}

int main(int argc, char** argv)
{
    return game::main(argc, argv);
}
