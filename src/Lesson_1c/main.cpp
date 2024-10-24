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