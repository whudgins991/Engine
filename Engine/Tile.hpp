#include <SFML/Graphics.hpp>
#include <iostream>

class Tile 
{
	int tile_type;
	int health;
	int grid_index;
	bool is_passable;

public:
	Tile();
	void is_occupied(bool occupied);
	bool get_passable();
	int get_tile_type();
	int get_health();
	void set_passable();
	void set_tile_type(int new_tile_type);
	void set_health(int new_health);

};