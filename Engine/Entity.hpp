#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>



class Entity {
	const sf::Texture* texture;
	//sf::Texture texture{ "Sprites/test_tile.png" };
	//sf::Texture texture;
	//sf::Sprite sprite;
	std::unique_ptr<sf::Sprite> sprite;
	sf::Vector2i grid_coordinates;
	sf::Vector2i cell_coordinates;
	sf::Vector2f render_scale;
	int number_of_tiles;
	int number_of_cells;
	float empty_space;
	int entity_type;
public:
	Entity();
	void set_spacing(float new_empty_space, int new_tiles_per_cell, int new_number_of_cells, sf::Vector2f new_render_scale);
	void set_entity_type(int new_entity_type, const sf::Texture* new_texture);
	void draw(sf::RenderWindow& window);
	sf::Vector2i get_grid_coordinates();
	sf::Vector2i get_cell_coordinates();
	void move(int x, int y, sf::Vector2i& current_cell, sf::Vector2i& current_world_coordinates);
	void set_grid_position(int new_x, int new_y, int new_a, int new_b);
};