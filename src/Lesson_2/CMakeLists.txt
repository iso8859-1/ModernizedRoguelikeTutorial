# modernizing to libtcod 1.24.0
add_executable(lesson2)
target_sources(lesson2 
	PRIVATE 
		main.cpp
		Actor.h
		Actor.cpp
		Map.h
		Map.cpp
		Engine.h
		Engine.cpp
)
target_link_libraries(lesson2 
	PRIVATE 
		libtcod::libtcod
		$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
		$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)