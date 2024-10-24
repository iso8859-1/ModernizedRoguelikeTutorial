#pragma once
#include "libtcod.hpp"

class Actor;
class Map;

class Engine {
public:
	Engine(int width, int height);
	~Engine();

	void update();
	void render(tcod::Console& console) const;

private:
	std::vector<std::unique_ptr<Actor>> actors_;
	Actor* player_; // player is not a unique_ptr because it is stored in actors_
	std::unique_ptr<Map> map_;
};