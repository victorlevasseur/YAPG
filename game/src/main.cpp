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
    lua::LuaState luaState;
    luaState.getState().Load("level.lua");
    std::string aa = luaState.getState()["level"][1]["parameters"]["width"];
    std::cout << "x = " << (int)(luaState.getState()["level"][1]["components"]["Position"]["x"]) << std::endl;
    std::cout << "height = " << (int)(luaState.getState()["level"][1]["components"]["Position"]["height"]) << std::endl;
    std::cout << "parameters.width = " << aa << std::endl;

    /*state::StateEngine stateEngine;
    window::WindowManager windowManager(stateEngine, "Yet Another Platformer Game");

    stateEngine.stopAndStartState<state::LevelState, std::string>("test.level");

    windowManager.run();*/

    return 0;
}

}

int main(int argc, char** argv)
{
    return game::main(argc, argv);
}
