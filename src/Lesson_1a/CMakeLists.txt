add_executable(lesson1a)
target_sources(lesson1a PRIVATE main.cpp)
target_link_libraries(lesson1a PRIVATE libtcod::libtcod)