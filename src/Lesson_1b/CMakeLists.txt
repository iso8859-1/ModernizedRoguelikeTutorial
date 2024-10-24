# moving @
add_executable(lesson1b)
target_sources(lesson1b PRIVATE main.cpp)
target_link_libraries(lesson1b PRIVATE libtcod::libtcod)