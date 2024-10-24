# Map and Actors

In this lesson, we'll create the base classes for the graphics display of the game. These classes are:

- Actor: contains the information how to render movable entities
- Map: contains the information how to render static entities
- Engine: controls the update(= move entities) and render(= draw entities) process. The game loop will `update()` and `render()` in each iteration of the game loop.

## Actor

We will create 2 files (`Actor.h` and `Actor.cpp`) in our project and add them to the `CMakeLists.txt` - in the `target_sources` command.

```
target_sources(lesson2 
	PRIVATE 
		main.cpp
		Actor.h
		Actor.cpp
)
```

In C++, cpp files are compiled independently. To be able to use contents of one cpp file in another, the important parts for the compiler need to be moved to the header file. For the `Actor` class, do the following:

```C++
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
```
- `pragma once` prevents that a header file is included multiple times into the same C++ file. This is important because otherwise the compiler sees multiple times the same symbol name and bails out.
- `class Actor`: defines a class. Classes (and structs) are a way to define your own types with their own content and semantics. In our case, we define a type named `Actor` that contains a position, a string for the symbol to render and a color information.
- `public`: this keywords indicates elements of the class that can be used by others. Elements in the `private` section of a class can only be accessed in methods of that class.
- `Actor(int x, int y, std::string ch, const tcod::ColorRGB& col);`: a function inside a class definition that has the same name as the class is a `constructor`. It is used to initialize the class. This constructor takes parameters for all class variables. The constructor intentionally has no body here. The body will be added later in the C++ file. The information inside the body is (usually) not needed in the header file since it is not required by the compiler to call a function. As a good rule of thumb, the compiler needs sizes of types (and thus their member variables), names of types and functions and parameter. These must be in a header file to be used.
- `void render(tcod::Console& console) const;`: a method of the type Actor. It takes a reference to the console. A reference is a way to hand an object into a function without copying it. Any modification to that object can also be observed outside. Once the function is over, you still have the modified object. References with the keyword `const` in front are a common way in C++ to avoid copying objects and still not worry about external modification. But since the render method modifies the console by rendering to it, the reference can't be const and it can't be a copy (otherwise the modifications inside would be lost after the function returns). The render method itself is const, because it does not modify the Actor's state.
- The methods `x`, `y`, `up`, ... are small inline methods. They have their body in the header file to allow the compiler to `inline` the method. This avoids some overhead and is good for the performance - but only for very small methods like these. If in doubt, do not inline a method because this increases the header file size and thus the compile time for any file that includes it.
- `x_`, `y_`, ... - member variables. These store the state of a class. For non-trivial classes, these should be private. In my code, I use the convention of appending an _ to the end of the class name. This is a visual help for the programmer to distinguish local variables from class state. There are many conventions about variable names - pick the one that suits you. But strife for consistency.

```c++
#include "Actor.h"

Actor::Actor(int x, int y, std::string ch, const tcod::ColorRGB& col) :
    x_(x), y_(y), ch_(std::move(ch)), col_(col) {
}

void Actor::render(tcod::Console& console) const {
	tcod::print(console, { x_, y_ }, ch_, col_, {});
}
```
This is the content of the Actor.cpp file. It contains the constructor definition (`Actor::Actor`) and the render method (`Actor::render`). The constructor initializes the class' state in the initializer list (following the :). This is preferable to initializing it in the body of the constructor (between the {}). The `std::move` for the `std::string` makes sure that the string is only copied once (in the function parameter) and that the copy is visible there. Alternativly, the parameter could also be of type `const std::string&` and then the initialization would look like this: `ch_(ch)`. Render simply calls the `print` function and fills its parameter from the Actor's state.

*From here on, I will only add the parts of the code to the document that are relevant for the explanation. The full code can always be found in the **src** folder in the subfolder for the lesson (e.g. Lesson_2 for this lesson)*

## Map

Similar to the Actor, Map is split into a Map.h and a Map.cpp file. These need to be added to the CMakelists.txt file.

```c++
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
```
- `#include <memory>` is required for `std::unique_ptr`.
- `Tile` is a struct (all members are public by default) and contains the state of the tiles. For now this is just the information if the tile is walkable or not.
- `Map` is our type to manage the map. It contains an array of tiles - one for each field on the map. Additionally, there is a helper method to detect whether a tile is a wall or not (the method does the coordinate to array index transformation) and a render method (like for the actor).
- `std::unique_ptr<Tile[]> tiles_` is a smart pointer to an array of Tile objects. The usage of the smart pointer ensures that the memory is freed when it is no longer needed. Since we use a `unique_ptr`, we **own** that memory and it is freed when the map is destroyed. Always try to use smart pointers to manage memory. It is almost always possible. And whenever you do, try to use `unique_ptr`. It is easy to understand and almost always the correct one.

```C++
Map::Map(int width, int height) :
	width_(width), height_(height),
	tiles_(std::make_unique<Tile[]>(width* height)) 
{
	setWall(30, 22);
	setWall(50, 22);
}
```
 In the constructor, `std::make_unique` creates the array. The number of elements is given but the function argument (width * height). Inside the body, the helper function is used to create a wall.

 A destructor is not needed (explicitly). The default generated by the compiler will be sufficient. This is due to the usage of types that manage their own memory like `std::string` and `std::unique_ptr` and not using raw pointer with new/delete.

 ```c++
 bool Map::isWall(int x, int y) const {
	return !tiles_[x + y * width_].canWalk;
}
```
Here you can see the calculation how x and y coordinates are transformed into the linear array index.

```c++
void Map::render(tcod::Console& console) const {
	constexpr tcod::ColorRGB darkWall = { 0, 0, 100 };
	constexpr tcod::ColorRGB darkGround = { 50, 50, 150 };

	for (int y = 0; y < height_; ++y) {
		for (int x = 0; x < width_; ++x) {
			console[{x, y}].bg = isWall(x, y) ? darkWall : darkGround;
		}
	}
}
```
The render method first defines 2 constants with the `constexpr` keyword. These are superior to `#define` since they are C++ constructs and not simply replaced in the pre-processor. Inside the method we reach into the `console` object and modify the background to all fields. If it's a wall, it will get the `darkWall` color otherwise the `darkGround` color.

## Engine

```c++
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
```

`class Actor;` is a forward declaration. It allows you to use the name without the need of including the header file. This is possible whenever the compiler only needs the name and is a technique to reduce compile time. This works because we use `Actor` only as template argument for the `unique_ptr` and don't try to access it. `Actor*` is a raw pointer. Since we will store a unique_ptr of the player in the `actors_` member, we can't use a `unique_ptr` to the same object a second time. But we can use a raw pointer. This is OK since Engine stores the raw pointer and owns the object.

A `std::vector` is an array of dynamic size. It can grow on demand. In our case, it will contain all of our `actors_`.

```c++
Engine::Engine(int width, int height) 
	: map_(std::make_unique<Map>(width, height))
{
    actors_.push_back(std::make_unique<Actor>(40, 25, "@", tcod::ColorRGB{ TCODColor::white }));
	player_ = actors_.back().get();
    actors_.push_back(std::make_unique<Actor>(60, 13, "@", tcod::ColorRGB{ TCODColor::yellow }));
}

Engine::~Engine() = default;
```

The constructor allocates the `Map` in the initializer list. The body adds a player-actor and a non-player. The player-actors pointer is also stored in `player_`. The destructor here is needed (and it is needed in the C++ file) even though it is a default destructor (indicated by `= default`). This is due to `Map` and `Actor` only fully known here in the cpp file where `Map.h` and `Actor.h` are included. If we put the destructor in the header file or leave it out to be created by the compiler by default, the compiler needs the full object information of `Map` and `Actor` in the header file - and the forward declaration would not be required.

```c++
void Engine::render(tcod::Console& console) const {
    TCOD_console_clear(console.get());
    map_->render(console);
	for (const auto& actor : actors_) {
		actor->render(console);
	}
}
```

The render method clears the console and then calls first render for the map and then render for each actor.

```c++
case SDLK_UP:
    if (!map_->isWall(player_->x(), player_->y() - 1))
    {
        player_->up();
    }
    break;
```
The `Engine::update` method contains the event handling with SDL2. The only difference to the previous lesson is the check for the wall. The player coordinates will only be updated if the field they walk into is not a wall.

## tying it together in main

```c++
constexpr int width = 80;
constexpr int height = 50;

Engine engine(width, height);

while (true)
{
    engine.update();
    engine.render(console);
	context.present(console);
}
```

The main method is now simpler. The event handling is moved out into the engine and the game loop is just `update`, `render` and then `present` to update the screen. It is also a good idea to move constant values into named constants - especially if they are used at multiple locations.