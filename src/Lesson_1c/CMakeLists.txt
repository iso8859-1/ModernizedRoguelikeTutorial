# modernizing to libtcod 1.24.0
add_executable(lesson1c)
target_sources(lesson1c PRIVATE main.cpp)
target_link_libraries(lesson1c 
	PRIVATE 
		libtcod::libtcod
		$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
		$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)