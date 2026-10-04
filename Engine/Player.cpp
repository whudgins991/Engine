#include "Player.hpp"

Player::Player()
{
	grid_position = { 0, 0 };
	render_scale = { 1, 1 };
	empty_space = 0;
	number_of_tiles = 1;
	number_of_cells = 1;
}
void Player::set_spacing(float new_empty_space, int new_tiles_per_cell, int new_number_of_cells, sf::Vector2f new_render_scale)
{
	empty_space = new_empty_space;
	number_of_tiles = new_tiles_per_cell;
	number_of_cells = new_number_of_cells;
	render_scale = new_render_scale;
	sprite.setPosition({ (render_scale.x * grid_position.x) + empty_space, render_scale.y * grid_position.y });
	sprite.setScale({ render_scale.x / 16, render_scale.y / 16 });

}
void Player::draw(sf::RenderWindow& window) 
{
    window.draw(sprite);
}
sf::Vector2i Player::get_grid_position()
{
	return grid_position;
}
sf::Vector2f Player::get_position() 
{
	return sprite.getPosition();
}
int Player::get_grid_position_x()
{
	return grid_position.x;
}
int Player::get_grid_position_y()
{
	return grid_position.y;
}
void Player::move(int x, int y, sf::Vector2i &current_cell, sf::Vector2i &current_world_coordinates) 
{

	grid_position.x += x;
	grid_position.y += y;
	if (grid_position.x < 0)
	{
		current_cell.x--;
		grid_position.x = number_of_tiles - 1;

	}
	if (grid_position.x >= number_of_tiles)
	{
		current_cell.x++;
		grid_position.x = 0;
	}
	if (grid_position.y < 0)
	{
		current_cell.y--;
		grid_position.y = number_of_tiles - 1;
	}
	if (grid_position.y >= number_of_tiles)
	{
		current_cell.y++;
		grid_position.y = 0;
	}

	if (current_cell.x < 0)
	{
		current_cell.x = number_of_cells - 1;
		current_world_coordinates.x--;
	}
	if (current_cell.x > number_of_cells - 1)
	{
		current_cell.x = 0;
		current_world_coordinates.x++;
	}
	if (current_cell.y < 0)
	{
		current_cell.y = number_of_cells - 1;
		current_world_coordinates.y--;
	}
	if (current_cell.y > number_of_cells - 1)
	{
		current_cell.y = 0;
		current_world_coordinates.y++;
	}
	sprite.setPosition({ (render_scale.x * grid_position.x) + empty_space, render_scale.y * grid_position.y });
	//std::cout << "DEBUG: Current Cell = { " << current_cell.x << "," << current_cell.y << " }\n";
	//std::cout << "DEBUG: Current World Coords = { " << current_world_coordinates.x << "," << current_world_coordinates.y << " }\n";

}
void Player::set_grid_position(sf::Vector2i new_grid_position)
{
	grid_position = new_grid_position;
}

Player::~Player() 
{

}