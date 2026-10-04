#include "Tile.hpp"



Tile::Tile()
{
	tile_type = 0;
	health = 100;
	grid_index = 0;
	if (tile_type == 0)
	{
		is_passable = (false);
	}
	else
	{
		is_passable = true;
	}
}

void Tile::debug_randomize_tile()
{
	int random_tile_type = rand() % 3;
	tile_type = random_tile_type;
}
bool Tile::get_passable()
{
	return is_passable;
}
int Tile::get_tile_type()
{
	return tile_type;
}
int Tile::get_health()
{
	return health;
}
void Tile::set_passable(bool new_is_passable)
{
	is_passable = new_is_passable;
}
void Tile::set_tile_type(int new_tile_type)
{
	tile_type = new_tile_type;
	if (tile_type == 0)
	{
		is_passable = (false);
	}
	else
	{
		is_passable = true;
	}
}
void Tile::set_health(int new_health)
{
	health = new_health;
}

Tile::~Tile()
{
	
}