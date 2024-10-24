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