#include "Map.h"

Map::Map(int width, int height) :
	width_(width), height_(height),
	tiles_(std::make_unique<Tile[]>(width* height)) 
{
	setWall(30, 22);
	setWall(50, 22);
}

bool Map::isWall(int x, int y) const {
	return !tiles_[x + y * width_].canWalk;
}

void Map::setWall(int x, int y) {
	tiles_[x + y * width_].canWalk = false;
}

void Map::render(tcod::Console& console) const {
	constexpr tcod::ColorRGB darkWall = { 0, 0, 100 };
	constexpr tcod::ColorRGB darkGround = { 50, 50, 150 };

	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			console[{x, y}].bg = isWall(x, y) ? darkWall : darkGround;
		}
	}
}