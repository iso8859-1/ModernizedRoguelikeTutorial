#pragma once
#include "libtcod.hpp"
#include <string>

class Actor {
public:
    Actor(int x, int y, std::string ch, const tcod::ColorRGB& col);
    void render(tcod::Console& console) const;

	int x() const { return x_; }
	int y() const { return y_; }

	void up() { y_--; }
	void down() { y_++; }
	void left() { x_--; }
	void right() { x_++; }
private:
    int x_, y_; // position on map
    std::string ch_; // ascii code
	tcod::ColorRGB col_; // color
};