#include "Engine.h"
#include "SDL.h"
#include "libtcod.hpp"

#include "Actor.h"
#include "Map.h"

Engine::Engine(int width, int height) 
	: map_(std::make_unique<Map>(width, height))
{
    actors_.push_back(std::make_unique<Actor>(40, 25, "@", tcod::ColorRGB{ TCODColor::white }));
	player_ = actors_.back().get();
    actors_.push_back(std::make_unique<Actor>(60, 13, "@", tcod::ColorRGB{ TCODColor::yellow }));
}

Engine::~Engine() = default;

void Engine::update() {
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
            case SDLK_UP:
                if (!map_->isWall(player_->x(), player_->y() - 1))
                {
                    player_->up();
                }
                break;
            case SDLK_DOWN:
                if (!map_->isWall(player_->x(), player_->y() + 1))
                {
                    player_->down();
                }
                break;
                {
            case SDLK_LEFT:
                if (!map_->isWall(player_->x() - 1, player_->y()))
                {
                    player_->left();
                }
                break;
            case SDLK_RIGHT:
                if (!map_->isWall(player_->x() + 1, player_->y()))
                {
                    player_->right();
                }
                break;
            default: break;
                }
                break;
            }
        default:
            break;
        }
    }
}

void Engine::render(tcod::Console& console) const {
    TCOD_console_clear(console.get());
	map_->render(console);
	for (const auto& actor : actors_) {
		actor->render(console);
	}
}