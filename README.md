# YAPG
YAPG - Yet Another Platformer Game

## Dependencies
YAPG currently requires those libraries :

 - SFML (>= 2.3 and < 3.x)
 - entityx (= 1.2.0), *provided in libs/entityx directory*
 - Lua (>= 5.2)
 - boost system and filesystem (>= 1.58.0)

It also requires a C++14-compliant compiler. Note that the game is officially
built with **GCC 5.3 on Linux** and **TDM-GCC (MinGW) 5.1** on Windows.

## How to build ?
*This guide considers you're using GCC on Linux.*
In the next sections, ```"YourGenerator"``` can be replaced by ```"Makefiles"```
on Linux with GCC and ```"MinGW Makefiles"``` on Windows with TDM-GCC/MinGW.

### Build entityx
Entityx is provided in this repository in the ```libs/entityx``` folder.

```sh
cd libs/entityx
mkdir build
cd build
cmake .. -G "YourGenerator" BUILD_SHARED=0
make
```

### Install the dev packages of SFML, boost and Lua
Then, you need to install the dev packages of SFML, boost and lua. If your
distribution doesn't provides these packages (or not supported versions of them)
or you want to build YAPG on Windows, you have to build/download these libraries
yourself, see the next section. If your distribution provides them, you can skip
the next section and go to the **Build the game** section.

### Build/download the others libraries yourself

**SFML**

For SFML, download the source archive on the official website (*2.3.x is the
recommended version*) and extract it in the ```libs``` directory.

Then, go to the extracted directory and do:

```sh
cmake .. -G "YourGenerator"
make
```

**Boost**

For Boost, download it archive on the official website and extract it in
the ```libs``` directory.
Then, go to the extracted folder and follow the instructions at
[Boost start guide](http://www.boost.org/doc/libs/1_60_0/more/getting_started/index.html).

**Note:** Do not forget to put ```toolset=gcc``` in the ```b2``` commands on
Windows if you're building the game with TDM-GCC/MinGW on Windows.

**Lua**

As Lua is a C API, there are no compatibility problems between compiler
versions. That's why you can download the prebuilt packages on the
[Lua binaries website](http://lua-users.org/wiki/LuaBinaries). Just make sure
that the version you've downloaded is built for your OS and architecture (x86 or
x86_64) and contains the lua header and the lua library (```.dll```/```.so```).
Extract it in the ```libs/Lua``` folder.

### Build the game
Now, return to the project root directory and do:

```sh
mkdir build
cd build
cmake .. -G "YourGenerator"
```

If you manually built the libraries, it will show some errors as the script
can't find the libraries in the system. You'll need to enter the path to the
libraries yourself before continuing. You can do it in CMake GUI or rewrite the
last command line with all the paths to configure the project properly.

Here are the path you must fill to build the game in Release mode:

 - ```Boost_INCLUDE_DIR``` => to the root directory of Boost (the script will be
     able to find the libraries itself with it)
 - ```LUA_INCLUDE_DIR``` => to the ```include``` directory of Lua
 - ```LUA_LIBRARY``` => to the luaXX.dll/lua.so.X.X shared library
 - ```SFML_XXX_LIBRARY_DYNAMIC_RELEASE``` => to the corresponding SFML shared
   library (XXX is AUDIO, GRAPHICS, SYSTEM, WINDOW)
 - ```SFML_INCLUDE_DIR``` => to the ```include``` directory of SFML

Execute the command again with these parameters (or click on "Configure", then
"Generate" in CMake GUI).

Eventually, do:
```sh
make install
```

and wait until the game is built and installed. The ```install``` command does
not copy the game to the system paths, it just copies the game executable and
the ressources (lua scripts, example levels, examples entities templates) in
the ```release``` directory inside the YAPG root directory.

### Play the game
Copy if needed the ```.dll```/```.so``` file into the ```release``` directory.

On Windows, just launch the ```yapg-game.exe``` file and on Linux, launch
the ```./yapg-game``` file.

On Linux, if you manually built the libraries (SFML, Boost and Lua), you may
need to execute this command before launching the game:

```sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
```

(To tell Linux to search for some shared libraries in the executable folder).
