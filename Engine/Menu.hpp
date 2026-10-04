#include <SFML/Graphics.hpp>
#include <iostream>

class Menu
{
	int menu_index;
	int menu_selection;
	sf::Font font{ "Fonts/AldotheApache.ttf" };
	sf::RectangleShape background;
	std::vector<sf::Text> menu_text;

public:
	Menu();
	void set_menu_index(int new_index);
	void set_position(sf::Vector2f new_position);
	void set_background_size(sf::Vector2f new_size);
	void set_fill_color(sf::Color new_color);
	void set_scale(sf::Vector2f new_scale);
	void set_outline_color(sf::Color new_color);
	void set_menu_text(std::vector<std::string> new_menu_text);
	int get_menu_index();
	sf::Vector2f get_size();
	void move_up();
	void move_down();
	int select();
	void menu_change();
	void draw(sf::RenderWindow& window);
	~Menu();

};