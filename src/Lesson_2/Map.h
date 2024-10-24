#pragma once
#include "libtcod.hpp"
#include <memory>

struct Tile {
	bool canWalk = true;
};

class Map {
public:
	Map(int width, int height);

	bool isWall(int x, int y) const;
	void render(tcod::Console& console) const;

private:
	void setWall(int x, int y);

	int width_;
	int height_;
	std::unique_ptr<Tile[]> tiles_;
};