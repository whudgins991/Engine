#include <SFML/Graphics.hpp>
#include <iostream>



class Player {
 sf::Texture texture{ "Sprites/player_sprite.png" };
 sf::Sprite sprite{ texture };
 sf::Vector2i grid_position;
 sf::Vector2f render_scale;
 int number_of_tiles;
 int number_of_cells;
 int empty_space;
public:
    Player();
	void set_spacing(float new_empty_space, int new_tiles_per_cell, int new_number_of_cells, sf::Vector2f new_render_scale);
    void draw(sf::RenderWindow& window);
	sf::Vector2i get_grid_position();
	int get_grid_position_x();
	int get_grid_position_y();
    sf::Vector2f get_position();
	void move(int x, int y, sf::Vector2i &current_cell, sf::Vector2i &current_world_coordinates);
	void set_grid_position(sf::Vector2i new_grid_posiion);
    ~Player();
};