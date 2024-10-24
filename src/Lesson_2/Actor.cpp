#include "Actor.h"

Actor::Actor(int x, int y, std::string ch, const tcod::ColorRGB& col) :
    x_(x), y_(y), ch_(std::move(ch)), col_(col) {
}

void Actor::render(tcod::Console& console) const {
	tcod::print(console, { x_, y_ }, ch_, col_, {});
}