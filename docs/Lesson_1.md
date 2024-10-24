# Lesson 1

This tutorial tries to be platform independent. CMake and vcpkg exist for all platforms and can be integrated into the IDE / compiler combination of your choice. The setup instructions are necessarily dependent on your personal choice of platform and IDE. Please provide pull-requests if you want to add your combination to the documentation.

## Setup the work environment

### Visual Studio / Windows

1. Install Visual Studio (https://visualstudio.microsoft.com/de/downloads/). The Community version is free and sufficient. Make sure to install the "Desktop Development with C++" workload and ensure in "Individual Components" that `vcpkg` and `CMake` are installed. This will install the Visual Studio IDE (= Integrated Development Environment). It is an application that integrates a text editor for editing the source code, a build tool (CMake), a package manager (vcpkg) and a debugger.
2. Open the `x64 Native Tools Command Prompt` and execute `vcpkg integrate install`. This integrates vcpkg into Visual Studio.
3. (Optional) Install `git` (https://visualstudio.microsoft.com/de/downloads/). Git is the defacto standard version control system. Don't do software development without a version control system. It allows you to try thing out and revert all changes in case you messed up. Visual Studio contains a git client that allows you to work with git without using the command line. Check the documentation to understand the basics.

Now you are ready to write C++ applications.

### Setting up the repository

- Create an empty git repository (e.g. https://git-scm.com/book/en/v2/Git-Basics-Getting-a-Git-Repository). 
- Call `vcpkg new --application`. This will create a file called `vcpkg-configuration.json` and a file `vcpkg.json`. The configuration defines where vcpkg fetches the package recipes from and which version of that registry is relevant for you. In `vcpkg.json` *you* define the dependencies of your project.
- Add the following files:

**`.gitignore`**

This file determines which files are relevant for git and which files aren't. You typically want to exclude and automatically generated files from either your build or your IDE - as they can be re-generated and potentially differ from computer to computer.

```
.vs/
out/
```
The above file will exclude the standard folders that Visual Studio generates. If you have a different IDE, you might want to change the `.gitignore` file to suit your needs (https://git-scm.com/book/en/v2/Git-Basics-Recording-Changes-to-the-Repository - section "Ignore files").

**CMakeLists.txt**

A file with this name in the root of your repository is the entry point for CMake to read your build definition. The build definition defines which executables and libraries you want to build, which source files are needed for this and which libraries to link.

```
cmake_minimum_required(VERSION 3.19)
project(RogueLikeTutorial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#find_package(libtcod CONFIG REQUIRED)

add_subdirectory(src)
```

- cmake_minimum_required defines the minimum version of CMake to use to compile. As a guideline, you should use at least a 3.x version as there were same significant quality of life features introduced in "modern" CMake.
- project defines the name of the project. This is not the executable name. You can create (and usually you do) multiple executables and libraries within the same tree of CMake files (see add_subdirectory below). It is also possible to do this within the same CMakeLists.txt file.
- set() writes to a CMake variable. The set instructions above write to predefined variables of CMake that define the C++ standard version (here 20), whether that is just a suggestion or a requirement and whether vendor extension of C++ should be allowed or not. For cross-platform development, you want to turn that OFF.
- find_package() tries to locate a library on your system. Once located, it can be used by your applications and libraries to link against. Here we want to find the libtcod library. It will be provided by vcpkg and Visual Studio will automatically configure CMake (via the toolchain file) to integrate vcpkg into the CMake build - so it will be found. For our first step, we will comment this out until we verified that our base configuration is working. This is done with the `#`-sign in front of the command. If your editor has syntax highlighting for CMake-files, it should be shown in a different color.
- add_subdirectory. CMake allows you to split your build definition across multiple `CMakeList.txt` files in different subdirectories. The command will prompt CMake to open the mentioned subdirectory and read the contained `CMakeLists.txt` file. This allows you to structure your build efficiently.

- Create a subdirectory **src** with the following files

**CMakeLists.txt**

This file defines now our application. This repository adds another layer of subdirectories for the different lessons - but if you stick to a single application, this step is not required.

```
add_executable(lesson1)
target_sources(lesson1 PRIVATE main.cpp)
#target_link_libraries(lesson1 PRIVATE libtcod::libtcod)
```

- add_executable defines a target that creates an executable with the name `lession1`
- target_sources adds to the target the file `main.cpp` as private file. The distinction between private and public files is irrelevant for this tutorial and kept out. It will be relevant though if you create libraries that other targets consume. You can have multiple files in the list if your application consists of multiple source files (which is usually the case)
- target_link_libraries links libraries to a target. Here `libtcod::libtcod` is linked to `lesson1`. Again, private/public is out of scope an mainly relevant when you create libraries. For the first run, this is commented out (`#`) sign.

**main.cpp**

The source file for our application.

```c++
#include <iostream>

int main()
{
    std::cout << "Hello World!\n";
    return 0;
}
```

THE hello world program for C++.

### Testing the build chain

Once everything is set-up as above:

- Open Visual Studio
- Select `Open Local Folder` - which is where the CMake integration is hiding
- Wait until the output in the output window reports success or failure.
- If it is a success, compile and run the application by pressing the button on the top of the screen with the green play-triangle and `lesson1.exe` behind it. If it doesn't say `lesson1.exe` try opening the drop-down menu and select it. Your application should now compile and show a command line window that says `Hello World!`
- If it wasn't a success, you need to read the output and try to fix the error. Check your installation. Check all the steps above and the contents of the files. Try opening this repository instead and see if that succeeds. If it does, there is an error in your code. If it doesn't, there's probably an error in your installation.

## Using LibTCOD

### linking libtcod

To be able to link libtcod to your application, you need to do the following steps:

1. Add libtcod to your dependencies. It should look similar to this:

```json
{
    "dependencies": [
        {
            "name": "libtcod",
            "features": ["unicode", "sdl"]
        }
    ]
}
```
The dependencies json-array contains all the libraries that you want to use in any application/library of your build. If you don't want to set any features, you can simply add a string with the library name in the dependencies array. If you want to enable certain features, it needs to be done like shown above. Here we enable the features `unicode` and `sdl` from libtcod. The features for each library can be looked-up on https://vcpkg.io in the package database.

2. Use `find_package` to find libtcod. If you followed the steps above, you just need to remove the `#` sign in your root CMakelists.txt

```
cmake_minimum_required(VERSION 3.19)
project(RogueLikeTutorial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(libtcod CONFIG REQUIRED)

add_subdirectory(src)
```

3. Use `target_link_libraries` to link against libtcod. If you followed the steps above, you just need to remove the `#` sign in your application CMakelists.txt

```
add_executable(lesson1)
target_sources(lesson1 PRIVATE main.cpp)
target_link_libraries(lesson1 PRIVATE libtcod::libtcod)
```

After you have done these changes, you should re-compile and see if your application still works the way it should (print "Hello World!")

### using libtcod (Lesson 1a)

Change your `main.cpp` to the following:

```c++
#include "libtcod.hpp"

int main() {
    TCODConsole::initRoot(80,50,"libtcod C++ tutorial",false);
    while ( !TCODConsole::isWindowClosed() ) {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr);
        TCODConsole::root->clear();
        TCODConsole::root->putChar(40,25,'@');
        TCODConsole::flush();
    }
    return 0;
}
```

**Explanation**

- `#include "libtcod.hpp"`: imports the symbols (classes, functions, ...) from libtcod. This makes the compiler aware what is available in libtcod when compiling this file.
- `int main()`: the main entry point of your application. In a executable, there must be a single main function.
- `TCODConsole::initRoot`: a static function that initializes the display. It is an 80 character width, 50 character height console created in a new window with the title "libcod C++ tutorial". It uses the default font and the `false` indicates that it does not start in fullscreen mode. You can play around with those values and see how the behavior of the application changes.
- `while(!TCODConsole::isWindowClosed()) { <loop content> }`: this is a while loop. A while loop checks the condition inside the () and if true, runs the code inside the {}. Once that is done, it checks the condition again and runs it again if true. If false, it continues after the }. This while-loop is called the *game loop*. The game loop is run as fast as possible to allow for a smooth game. Every run of the game loop refreshes the screen one time. The condition states that it is run until the window is closed.
- `TCOD_key_t key;`: defines a variable that will hold which key was pressed.
- `TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);`: libtcod requires this function to be called to process screen redraw events. The address of the previously generated variable `key` is passed into the function. This allows the event function to write into the variable. The last parameter allows mouse processing, but isn't used in this program.  
- `TCODConsole::root->clear()`: accesses the global object `root` and calls the function `clear()` on it. `root` contains the console bound to the main window. `clear()`clears the screen.
- `TCODConsole::root->putChar(40,25,'@')`: writes the character `@` at location 40, 25 on the screen (in the middle)
- `TCODConsole::flush()`: To speed up drawing, all drawing commands are stored in a buffer. `flush()` actually takes the stored drawings and brings them to the screen.

Compile and run the program and you'll see an @ character in the middle of a black window.

### the walking '@'

Now modify (or create a new project) the contents of `main.cpp` to contain this:

```c++
#include "libtcod.hpp"
int main() {
    int playerx = 40, playery = 25;
    TCODConsole::initRoot(80, 50, "libtcod C++ tutorial", false);
    while (!TCODConsole::isWindowClosed()) {
        TCOD_key_t key;
        TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, nullptr);
        switch (key.vk) {
        case TCODK_UP: playery--; break;
        case TCODK_DOWN: playery++; break;
        case TCODK_LEFT: playerx--; break;
        case TCODK_RIGHT: playerx++; break;
        default:break;
        }
        TCODConsole::root->clear();
        TCODConsole::root->putChar(playerx, playery, '@');
        TCODConsole::flush();
    }
    return 0;
}
```

This is a slight modification of the previous program:

1. the `@` is drawn at the location of the variables `playerx` and `playery`.
2. we use the read key value in a switch statement to modify `playerx`, `playery`. When you press one of the arrow-keys, the coordinats are modified. Please note, up is negative y - which might not be intuitive. This is the opposite direction from the coordinate systems you know from math classes.

Compile and run. Now you can move the `@` sign across the screen.

### modernizing to the new API

The old tutorial was written against an older version of libtcod. The current version (1.24.0) of the library deprecated many of the used APIs and provided some migration information. The major differences is the existance of contexts and using the SDL event loop and not the libtcod event mechanism. To modernize to the new API, the following modifications need to be done:

**Extend vcpkg.json to include SDL2**

```json
{
    "dependencies": [
        {
            "name": "libtcod",
            "features": [ "unicode", "sdl" ]
        },
        "sdl2"
    ]
}
```
**link against SDL2**

root CMakeLists.txt:
```
find_package(SDL2 CONFIG REQUIRED)
```

application CMakeLists.txt:
```
# modernizing to libtcod 1.24.0
add_executable(lesson1c)
target_sources(lesson1c PRIVATE main.cpp)
target_link_libraries(lesson1c 
	PRIVATE 
		libtcod::libtcod
		$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
		$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
```
The link command looks strange since it uses CMake generate expressions. There's no need for deeper understanding of this in the tutorial - the code was taken from the install information that vcpkg provides when running install.

**the updated code**

```c++
#include <SDL.h>
#include "libtcod.hpp"

//modernized according to: https://libtcod.readthedocs.io/en/latest/guides/getting-started.html#getting-started
int main(int argc, char** argv) {
    auto console = tcod::Console{ 80, 50 };

	auto params = TCOD_ContextParams{};
	params.tcod_version = TCOD_COMPILEDVERSION; //required by library
	params.console = console.get(); //derive the window size from the console size
	params.window_title = "libtcod C++ tutorial";
	params.sdl_window_flags = SDL_WINDOW_RESIZABLE; //optional
	params.vsync = true;
	params.argc = argc;
	params.argv = argv;

	auto context = tcod::Context{ params };

    int playerx = 40, playery = 25;
    while (true)
    {
        //render
        TCOD_console_clear(console.get());
        tcod::print(console, { playerx, playery }, "@", { {255,255,255} }, { { 0,0,0 } });
        context.present(console);

		//process input
		SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                std::exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_UP: playery--; break;
                case SDLK_DOWN: playery++; break;
                case SDLK_LEFT: playerx--; break;
                case SDLK_RIGHT: playerx++; break;
                default: break;
                }
                break;
            default:
                break;
            }
        }
    }
    return 0;
}
```

Explanation:
- `#include <SDL.h>`: required since we need SDL2 for processing the events.
- `int main(int argc, char** argv)`: this version of main allows processing of command line arguments. In this example they can be used to configure the SDL window.
- `auto console = tcod::Console{80, 50};`: constructs a `tcod::Console` object. `tcod` is a namespace that avoids name collisions with other libraries or your program. `auto` means that the compiler should deduce the actual type of the object here and use that so that you don't have to write it out.
- `auto params = ...`: The whole `params` section configures the window to be shown.
- `auto context = ...`: creates a context object. The context allows interaction with the window. Most important here - rendering the content of the console to screen.
- `while (true) {}`: the game loop. Now as infinite loop. Exit from the loop is done during event handling.
- `TCOD_console_clear(console.get());`: clears the console
- `tcod::print(console, { playerx, playery }, "@", { {255,255,255} }, { { 0,0,0 } });`: writes a string to the console at the givven row/colum (these are not pixels). It can handle multiple characters and unicode. The odd looking number-triples are optional colors as RGB. They require two sets of braces because they can also be empty - in this case the current color will be used.
- `context.present(console);`: renders the contents of the console to the window represented by the context.
- `while (SDL_PollEvent(&event))`: event handling with SDL2. Here as loop to handle all events that happened during the previous cycle of the game loop. If you have no animation (e.g. all changes to the screen are caused by some key/mouse events), you can use `SDL_WaitEvent(nullptr);` before the while loop to sleep until the next event. Here `SDL_QUIT` handles the windows close by calling `std::exit(0)` to exit the application with exit code 0. The rest ist keyboard event handling using SDL2 with the SDL2 instead of the libtcod constants and functions.


