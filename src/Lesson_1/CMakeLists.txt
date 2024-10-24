add_executable(lesson1)
target_sources(lesson1 PRIVATE main.cpp)
target_link_libraries(lesson1 PRIVATE libtcod::libtcod)