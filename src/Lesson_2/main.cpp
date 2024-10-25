#include <SDL.h>
#include "libtcod.hpp"

#include "Engine.h"

//modernized according to: https://libtcod.readthedocs.io/en/latest/guides/getting-started.html#getting-started
int main(int argc, char** argv) {
	constexpr int width = 80;
	constexpr int height = 50;

    auto console = tcod::Console{ width, height };

	auto params = TCOD_ContextParams{};
	params.tcod_version = TCOD_COMPILEDVERSION; //required by library
	params.console = console.get(); //derive the window size from the console size
	params.window_title = "libtcod C++ tutorial";
	params.sdl_window_flags = SDL_WINDOW_RESIZABLE; //optional
	params.vsync = true;
	params.argc = argc;
	params.argv = argv;

	auto context = tcod::Context{ params };

	Engine engine(width, height);

    while (true)
    {
		engine.update();
		engine.render(console);
		context.present(console);
    }
    return 0;
}