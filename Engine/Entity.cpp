#include "Entity.hpp"

Entity::Entity()
{
	grid_coordinates = { 0, 0 };
	cell_coordinates = { 0, 0 };
	render_scale = { 1, 1 };
	empty_space = 0;
	number_of_tiles = 1;
	number_of_cells = 1;
	entity_type = 0;
	texture = nullptr;
}
void Entity::set_spacing(float new_empty_space, int new_tiles_per_cell, int new_number_of_cells, sf::Vector2f new_render_scale)
{
	empty_space = new_empty_space;
	number_of_tiles = new_tiles_per_cell;
	number_of_cells = new_number_of_cells;
	render_scale = new_render_scale;
	if (sprite)
	{
		sprite->setPosition({ (render_scale.x * grid_coordinates.x) + empty_space, render_scale.y * grid_coordinates.y });
		sprite->setScale({ render_scale.x / 16, render_scale.y / 16 });
	}

}
void Entity::set_entity_type(int new_entity_type, const sf::Texture* new_texture)
{
	entity_type = new_entity_type;
	texture = new_texture;
	//bool failed_to_load = false;
	//switch (entity_type)
	//{
	//case 0:
	//	if (!texture.loadFromFile("Sprites/test_tile.png"))
	//	{
	//		failed_to_load = true;
	//	}
	//	break;
	//case 1:
	//	if (!texture.loadFromFile("Sprites/enemy_crab_sprite.png"))
	//	{
	//		failed_to_load = true;
	//	}
	//	break;
	//}
	//if (failed_to_load)
	//{
	//	std::cout << "DEBUG: Failed to load texture for entity type " << entity_type << '\n';
	//}
	if (texture)
	{
		sprite = std::make_unique<sf::Sprite>(*texture);
	}
}

void Entity::draw(sf::RenderWindow& window)
{
	if (sprite)
	{
		window.draw(*sprite);
	}
}
sf::Vector2i Entity::get_grid_coordinates()
{
	return grid_coordinates;
}
sf::Vector2i Entity::get_cell_coordinates()
{
	return cell_coordinates;
}
void Entity::move(int x, int y, sf::Vector2i& current_cell, sf::Vector2i& current_world_coordinates)
{

	//TODO: Implement movement logic and pathing AI

}
void Entity::set_grid_position(int new_x, int new_y, int new_a, int new_b)
{
	grid_coordinates = { new_a, new_b };
	cell_coordinates = { new_x, new_y };
}

