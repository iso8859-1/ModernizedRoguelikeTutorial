cmake_minimum_required(VERSION 3.19)
project(RogueLikeTutorial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(libtcod CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_subdirectory(src)