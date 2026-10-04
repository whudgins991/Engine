#include "main.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include "Menu.hpp"

sf::RenderWindow window;
//Set the number of rows/columns in current grid. Chaning this will change the arrays/vectors further in. Note: Unexpected things may happen if this is not a multiple of 3
const int tiles_per_grid = 81;
//Set the number of cells (screens) per grid (loaded tile sets). This can also be changed
const int number_of_cells = 3;
//Set how large each tile appears on screen
sf::Vector2f render_scale;
Player player;
//Used for control logic
bool key_pressed = false;
std::chrono::milliseconds current_time;
double timer;
double last_time;
sf::Keyboard::Key last_key_pressed;
//The current cell appears as a square on the screen. This calculates the empty space on the sides needed to make a square
float empty_space;
//This is the currently loaded grid
Tile tile_grid[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3];
Tile temp_grid[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3];
//Textures
sf::Texture test_tile_texture("sprites/test_tile.png");
sf::Texture grass_texture("Sprites/grass_sprite.png");
sf::Texture stone_texture("Sprites/stone_sprite.png");
sf::Texture dirt_texture("Sprites/dirt_sprite.png");
sf::Texture zero_texture("Sprites/DEBUG_0.png");
sf::Texture one_texture("Sprites/DEBUG_1.png");
sf::Texture two_texture("Sprites/DEBUG_2.png");
sf::Texture three_texture("Sprites/DEBUG_3.png");
sf::Texture four_texture("Sprites/DEBUG_4.png");
sf::Texture five_texture("Sprites/DEBUG_5.png");
sf::Texture six_texture("Sprites/DEBUG_6.png");
sf::Texture seven_texture("Sprites/DEBUG_7.png");
sf::Texture eight_texture("Sprites/DEBUG_8.png");
sf::Texture nine_texture("Sprites/DEBUG_9.png");
//Used to denote which screen is cell is rendered at any given time
sf::Vector2i projected_coordinates = { 0, 0 };
sf::Vector2i projected_cell = { 0, 0 };
sf::Vector2i projected_world_coordinates = { 0, 0 };
sf::Vector2i current_cell = { 0, 0 };
sf::Vector2i current_world_coordinates = { 0 , 0 };
sf::Vector2i last_cell = { 0, 0 };
sf::Vector2i last_world_coordinates = { 0, 0 };

//This is the 2d vector used to display the current cell
std::vector<std::vector<sf::Sprite>> sprite_vector;
std::string io_file_data_read;
std::string io_file_data_line;
std::string io_tile_type_string;
int tile_type;
std::vector<int> io_tile_type_vector;

std::random_device random;
std::mt19937 random_generator(random());

std::vector<int> tile_peek;

//sf::Font font("Fonts/AldotheApache.ttf");
//std::vector<sf::Text> menu_text;
//sf::RectangleShape pause_menu;
//sf::RectangleShape options_menu;
//bool menu_is_open = false;
//int which_menu_is_open = 0;
//int menu_selection = 0;
std::vector<Menu> menu_vector;
int menu_is_open = 0;
sf::Keyboard::Key key_pressed_storage;
bool is_fullscreen = true;
bool was_fullscreen = true;

void realign();
void set_tile_textures();
void debug_randomize_tiles(Tile (&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3]);
void read_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3]);
void write_new_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3]);
bool peek_next_grid(sf::Vector2i attempted_world_coordinates, sf::Vector2i attempted_cell, sf::Vector2i attempted_grid_coordinates);
void move_player(sf::Keyboard::Key movement_key_pressed, sf::Vector2i attempted_movement);
sf::Keyboard::Key get_key_pressed();



int main()
{
    // create the window
    window.create(sf::VideoMode({ 160, 160 }), "My window", sf::Style::Default, sf::State::Fullscreen);
	//sf::RenderWindow window(sf::VideoMode({ 2000, 1600 }), "My window", sf::Style::Default);

	empty_space = ((window.getSize().x - window.getSize().y) / 2);
	render_scale = { ((static_cast<float>(window.getSize().x) - (empty_space * 2)) / (tiles_per_grid/3)), (static_cast<float>(window.getSize().y) / (tiles_per_grid/3)) };
	//menu.setSize({ 10,15 });
	
	for (int x = 0; x < 4; x++)
	{
		Menu m;
		m.set_menu_index(x);
		menu_vector.push_back(m);
	}
	menu_vector[0].set_menu_text({ "" });
	menu_vector[1].set_menu_text({ "Resume", "Options", "Exit Game" });
	menu_vector[2].set_menu_text({ "Toggle Fullscreen", "Keybinds", "Back" });
	menu_vector[3].set_menu_text({ "test1", "test2", "Back" });
	for (int x = 0; x < menu_vector.size(); x++)
	{
		menu_vector[x].set_fill_color(sf::Color(0, 0, 0, 128));
		menu_vector[x].set_outline_color(sf::Color::Magenta);
		menu_vector[x].set_scale(render_scale);
		menu_vector[x].set_position({ static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2  });
	}

	//pause_menu.setSize({ static_cast<float>(window.getSize().x) / 250, static_cast<float>(window.getSize().y / 150)});
	//pause_menu.setFillColor(sf::Color(0, 0, 0, 128));
	//pause_menu.setScale(render_scale);
	//options_menu.setSize({ static_cast<float>(window.getSize().x) / 250, static_cast<float>(window.getSize().y / 150) });
	//options_menu.setFillColor(sf::Color(0, 0, 0, 128));
	//options_menu.setScale(render_scale);

	//pause_menu.setPosition({ static_cast<float>(window.getSize().x)/2 - (render_scale.x * static_cast<float>(pause_menu.getSize().x) / 2), static_cast<float>(window.getSize().y/2 - (render_scale.y * static_cast<float>(pause_menu.getSize().y)) / 2) });
	//
	//for (int x = 0; x < 3; x++)
	//{
	//	menu_text.push_back(sf::Text(font));
	//}
	//menu_text[0].setString("Resume");
	//menu_text[1].setString("Options");
	//menu_text[2].setString("Exit Game");
	//for (int x = 0; x < static_cast<int>(menu_text.size()); x++)
	//{
	//	menu_text[x].setScale({render_scale.x / 15, render_scale.y / 15});
	//	menu_text[x].setPosition({ pause_menu.getPosition().x + (render_scale.x * pause_menu.getSize().x / 2) - (menu_text[x].getGlobalBounds().size.x / 2), pause_menu.getPosition().y + (menu_text[x].getGlobalBounds().size.y * ((2 * x))) });
	//	menu_text[x].setOutlineColor(sf::Color::Magenta);
	//}
	//menu_text[menu_selection].setOutlineThickness(1);

	
	//Passes the player class all info needed
	player.set_spacing(empty_space, (tiles_per_grid/3),number_of_cells, render_scale);

	//DEBUG: randomize all tiles in the grid
	//debug_randomize_tiles();
	//Fills in the vector used for sprite rendering
	for (int x = 0; x < tiles_per_grid / 3; x++)
	{
		std::vector<sf::Sprite> v;
		sprite_vector.push_back(v);
		for (int y = 0; y < tiles_per_grid / 3; y++)
		{
			sprite_vector[x].push_back(sf::Sprite(test_tile_texture));
		}
	}
	read_grid(current_world_coordinates, tile_grid);
	set_tile_textures();

	//Sets the sprite to be displayed based on what kind of tile is being shown
	for (int x = 0; x < tiles_per_grid / 3; x++)
	{
		for (int y = 0; y < tiles_per_grid / 3; y++)
		{
			sprite_vector[x][y].setScale({ render_scale.x / 16, render_scale.y / 16 });
			sprite_vector[x][y].setPosition({ ((render_scale.x * x) + empty_space), render_scale.y * y });
			switch (tile_grid[current_cell.x][current_cell.y][x][y].get_tile_type())
			{
			case 0: sprite_vector[x][y].setTexture(stone_texture); break;
			case 1: sprite_vector[x][y].setTexture(dirt_texture); break;
			case 2: sprite_vector[x][y].setTexture(grass_texture); break;
			default: sprite_vector[x][y].setTexture(test_tile_texture); break;
			}
		}
	}
	

    // run the program as long as the window is open
	while (window.isOpen())
    {

		//Used to determine how long a button has been held for movement purposes
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		timer = static_cast<double>(current_time.count());


        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
			if (event->is<sf::Event::Resized>())
				realign();
        }
		//Used to determine if the cell has changed for drawing logic
		last_cell = current_cell;
		last_world_coordinates = current_world_coordinates;
		//Used to determine if a key is being held
		if (!sf::Keyboard::isKeyPressed(last_key_pressed))
		{
			key_pressed = false;
		}
		//If no movement keys are held down, reset the timer
		if (!key_pressed)
		{
			timer = 0;
		}
		////Close the game upon pressing Esc
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		//{
		//	window.close();
		//}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && !key_pressed)
		{
			last_key_pressed = sf::Keyboard::Key::Escape;
			key_pressed = true;
			if (menu_is_open > 0)
			{
				menu_is_open--;
				menu_vector[menu_is_open].menu_change();
			}
			else
			{
				menu_is_open = 1;
				menu_vector[menu_is_open].menu_change();

			}
			if (menu_is_open < 0)
			{
				menu_is_open = 0;
			}
		}
		if (menu_is_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8) && !key_pressed)
		{
			last_key_pressed = sf::Keyboard::Key::Numpad8;
			key_pressed = true;
			menu_vector[menu_is_open].move_up();
		}
		if (menu_is_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2) && !key_pressed)
		{
			last_key_pressed = sf::Keyboard::Key::Numpad2;
			key_pressed = true;
			menu_vector[menu_is_open].move_down();
		}

		//if (menu_is_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8) && !key_pressed)
		//{
		//	menu_text[menu_selection].setOutlineThickness(0);
		//	menu_selection--;
		//	if (menu_selection < 0)
		//	{
		//		menu_selection = static_cast<int>(menu_text.size()) - 1;
		//	}
		//	last_key_pressed = sf::Keyboard::Key::Numpad8;
		//	key_pressed = true;
		//	menu_text[menu_selection].setOutlineThickness(1);
		//}
		//if (menu_is_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2) && !key_pressed)
		//{
		//	menu_text[menu_selection].setOutlineThickness(0);
		//	menu_selection++;
		//	if (menu_selection > static_cast<int>(menu_text.size()) - 1)
		//	{
		//		menu_selection = 0;
		//	}
		//	last_key_pressed = sf::Keyboard::Key::Numpad2;
		//	key_pressed = true;
		//	menu_text[menu_selection].setOutlineThickness(1);
		//}
		//if (menu_is_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)&& menu_selection == 0 && !key_pressed)
		//{
		//	menu_text[menu_selection].setOutlineThickness(0);
		//	menu_is_open = false;
		//	last_key_pressed = sf::Keyboard::Key::Space;
		//	key_pressed = true;
		//	menu_text[menu_selection].setOutlineThickness(1);
		//}
		//if (menu_is_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && menu_selection == 1 && !key_pressed)
		//{
		//	which_menu_is_open = 1;
		//	last_key_pressed = sf::Keyboard::Key::Space;
		//	key_pressed = true;
		//}
		//if (menu_is_open && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && menu_selection == 2 && !key_pressed)
		//{
		//	window.close();
		//}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad1))
		{
			move_player(sf::Keyboard::Key::Numpad1, { -1, 1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2))
		{
			move_player(sf::Keyboard::Key::Numpad2, { 0, 1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad3))
		{
			move_player(sf::Keyboard::Key::Numpad3, { 1, 1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad4))
		{
			move_player(sf::Keyboard::Key::Numpad4, { -1, 0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad5))
		{
			move_player(sf::Keyboard::Key::Numpad5, { 0, 0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad6) )
		{
			move_player(sf::Keyboard::Key::Numpad6, { 1, 0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad7) )
		{
			move_player(sf::Keyboard::Key::Numpad7, { -1 , -1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad8) )
		{
			move_player(sf::Keyboard::Key::Numpad8, { 0, -1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad9) )
		{
			move_player(sf::Keyboard::Key::Numpad9, { 1 , -1 });
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !key_pressed && menu_is_open > 0)
		{
			last_key_pressed = sf::Keyboard::Key::Space;
			key_pressed = true;
			std::cout << "DEBUG: Menu selection is " << menu_vector[menu_is_open].select() << "\n";
			switch (menu_vector[menu_is_open].select())
			{
			case -2:
				is_fullscreen = !is_fullscreen;
				if (is_fullscreen)
				{
					window.create(sf::VideoMode({ 160, 160 }), "My window", sf::Style::Default, sf::State::Fullscreen);
					//sf::RenderWindow window(sf::VideoMode({ 160, 160 }), "My window", sf::Style::Default, sf::State::Fullscreen);
				}
				else
				{
					window.create(sf::VideoMode({ 2000, 1600 }), "My window", sf::Style::Default, sf::State::Windowed);
					//sf::RenderWindow window(sf::VideoMode({ 2000, 1600 }), "My window", sf::Style::Default);6
				}
				break;
			case -1:
				window.close();
				break;
			case 0:
				menu_is_open = 0;
				break;
			case 1:
				menu_is_open = 1;
				break;
			case 2:
				menu_is_open = 2;
				break;
			default:
				break;
			}
		}

		if (was_fullscreen != is_fullscreen)
		{
			realign();
		}

		//if (sf::Keyboard::isKeyPressed(key_pressed_storage) && !key_pressed)
		//{
		//	last_key_pressed = key_pressed_storage;
		//	key_pressed = true;
		//	player.set_spacing(empty_space, (tiles_per_grid / 3), number_of_cells, { render_scale.x * 3, render_scale.y * 3 });
		//}

		
		


		//DEBUG: Spacebar randomizes the tiles
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !key_pressed)
		//{
		//	key_pressed = true;
		//	last_key_pressed = sf::Keyboard::Key::Space;
		//	debug_randomize_tiles();
		//	set_tile_textures();
		//}
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !key_pressed)
		//{
		//	key_pressed = true;
		//	last_key_pressed = sf::Keyboard::Key::Space;
		//	read_grid();
		//}
	
        // clear the window with black color
        window.clear(sf::Color::Black);

        //Drawing logic follows
		//If the current cell has changed, update all onscreen sprites based on the new tiles on screen
		if (!(last_cell == current_cell))
		{
			set_tile_textures();
		}
		if (!(last_world_coordinates == current_world_coordinates))
		{
			std::cout << "World coords changed, reading new tileset\n";
			read_grid(current_world_coordinates, tile_grid);
			set_tile_textures();
		}
		//Draw all onscreen sprites
		for (int x = 0; x < tiles_per_grid / 3; x++)
		{
			for (int y = 0; y < tiles_per_grid / 3; y++)
			{
				window.draw(sprite_vector[x][y]);
			}
		}
		//if (menu_is_open)
		//{
		//	window.draw(pause_menu);
		//	for (int x = 0; x < menu_text.size(); x++)
		//	{
		//		window.draw(menu_text[x]);
		//	}
		//}
		//Draw the player
		player.draw(window);
		if (menu_is_open > 0)
		{
			menu_vector[menu_is_open].draw(window);
		}

        // end the current frame
        window.display();
    }
}

void realign()
{
	empty_space = ((window.getSize().x - window.getSize().y) / 2);
	render_scale = { ((static_cast<float>(window.getSize().x) - (empty_space * 2)) / (tiles_per_grid / 3)), (static_cast<float>(window.getSize().y) / (tiles_per_grid / 3)) };
	player.set_spacing(empty_space, (tiles_per_grid / 3), number_of_cells, render_scale);
	was_fullscreen = is_fullscreen;

	for (int x = 0; x < tiles_per_grid / 3; x++)
	{
		for (int y = 0; y < tiles_per_grid / 3; y++)
		{
			sprite_vector[x][y].setScale({ render_scale.x / 16, render_scale.y / 16 });
			sprite_vector[x][y].setPosition({ ((render_scale.x * x) + empty_space), render_scale.y * y });
		}
	}
	for (int x = 0; x < menu_vector.size(); x++)
	{
		menu_vector[x].set_scale(render_scale);
		menu_vector[x].set_position({ static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2 });
	}
}
//Sets the onscreen sprites based on the type of tile
void set_tile_textures()
{
	//std::cout << "DEBUG: Setting tile textures\n";

	for (int x = 0; x < tiles_per_grid / 3; x++)
	{
		for (int y = 0; y < tiles_per_grid / 3; y++)
		{
			//std::cout << "Attempting to set tile at tile_grid["<<current_cell.x<<"]["<<current_cell.y<<"]["<<x<<"]["<<y<<"]\n ";

			switch (tile_grid[current_cell.x][current_cell.y][x][y].get_tile_type())
			{
			case 0: sprite_vector[x][y].setTexture(stone_texture); tile_grid[current_cell.x][current_cell.y][x][y].set_passable(false); break;
			case 1: sprite_vector[x][y].setTexture(dirt_texture);tile_grid[current_cell.x][current_cell.y][x][y].set_passable(true); break;
			case 2: sprite_vector[x][y].setTexture(grass_texture);tile_grid[current_cell.x][current_cell.y][x][y].set_passable(true); break;
			case 10: sprite_vector[x][y].setTexture(zero_texture); break;
			case 11: sprite_vector[x][y].setTexture(one_texture); break;
			case 12: sprite_vector[x][y].setTexture(two_texture); break;
			case 13: sprite_vector[x][y].setTexture(three_texture); break;
			case 14: sprite_vector[x][y].setTexture(four_texture); break;
			case 15: sprite_vector[x][y].setTexture(five_texture); break;
			case 16: sprite_vector[x][y].setTexture(six_texture); break;
			case 17: sprite_vector[x][y].setTexture(seven_texture); break;
			case 18: sprite_vector[x][y].setTexture(eight_texture); break;
			case 19: sprite_vector[x][y].setTexture(nine_texture); break;
			default: sprite_vector[x][y].setTexture(test_tile_texture); break;
			}
		}
	}
	//std::cout << "DEBUG: tile textures successfully set\n";

}
//DEBUG: Randomizes all onscreen tiles
void debug_randomize_tiles(Tile (&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3])
{

	//int stone_chance;
	//stone_chance = (rand() % 10);
	int rand_x;
	int rand_y;
	int rand_a;
	int rand_b;
	//int temp_x;
	//int temp_y;
	int temp_a;
	int temp_b;
	std::uniform_int_distribution<> random_cells(0, number_of_cells - 1);
	std::uniform_int_distribution<> random_tiles(0, (tiles_per_grid / 3) - 1);
	std::uniform_int_distribution<> stone_chance(50, 500);
	std::uniform_int_distribution<> random_chance_to_continue(0, 1);
	int chance_to_continue;
	bool iteration;
	bool grew = false;


	for (int x = 0; x < number_of_cells; x++)
	{
		//std::uniform_int_distribution<> stone_chance(10, 4);
		for (int y = 0; y < number_of_cells; y++)
		{
			for (int a = 0; a < tiles_per_grid / 3; a++)
			{
				for (int b = 0; b < tiles_per_grid / 3; b++)
				{
					array_reference[x][y][a][b].set_tile_type(2);
				}
			}
		}
	}
	for (int i = 0;i < (stone_chance(random_generator)); i++)
	{

		rand_x = random_cells(random_generator);
		rand_y = random_cells(random_generator);
		rand_a = random_tiles(random_generator);
		rand_b = random_tiles(random_generator);
		//std::cout << "DEBUG: rand_x = " << rand_x << ".\n";
		//std::cout << "DEBUG: rand_y = " << rand_y << ".\n";
		//std::cout << "DEBUG: rand_a = " << rand_a << ".\n";
		//std::cout << "DEBUG: rand_b = " << rand_b << ".\n";
		//rand_x = (rand() % number_of_cells);
		//rand_y = (rand() % number_of_cells);
		//rand_a = (rand() % (tiles_per_grid / 3));
		//rand_b = (rand() % (tiles_per_grid / 3));
		if (!((rand_x == current_cell.x) && (rand_y == current_cell.y) && (rand_a == player.get_grid_position_x()) && (rand_b == player.get_grid_position_y())))
		{
			array_reference[rand_x][rand_y][rand_a][rand_b].set_tile_type(0);

			iteration = true;
			//chance_to_continue = random_chance_to_continue(random_generator);
			while (iteration == true)
			{
				grew = false;
				std::cout << "DEBUG: Growth loop\n";
				//temp_x = rand_x;
				//temp_y = rand_y;
				temp_a = rand_a;
				temp_b = rand_b;
				chance_to_continue = random_chance_to_continue(random_generator);

				if (rand_a > 0 && array_reference[rand_x][rand_y][rand_a - 1][rand_b].get_tile_type() == 2)
				{
					if (chance_to_continue == 0)
					{
						array_reference[rand_x][rand_y][rand_a - 1][rand_b].set_tile_type(0);
						grew = true;
						temp_a = rand_a - 1;
					}

				}
				if (rand_a < ((tiles_per_grid / 3) - 1) && (array_reference[rand_x][rand_y][rand_a + 1][rand_b].get_tile_type() == 2))
				{
					if (chance_to_continue == 0)
					{
						array_reference[rand_x][rand_y][rand_a + 1][rand_b].set_tile_type(0);
						grew = true;
						temp_a = rand_a + 1;
					}

				}
				if (rand_b > 0 && array_reference[rand_x][rand_y][rand_a][rand_b - 1].get_tile_type() == 2)
				{
					if (chance_to_continue == 0)
					{
						array_reference[rand_x][rand_y][rand_a][rand_b - 1].set_tile_type(0);
						grew = true;
						temp_b = rand_b - 1;
					}

				}
				if (rand_b < (tiles_per_grid / 3) - 1 && array_reference[rand_x][rand_y][rand_a][rand_b + 1].get_tile_type() == 2)
				{
					if (chance_to_continue == 0)
					{
						array_reference[rand_x][rand_y][rand_a][rand_b + 1].set_tile_type(0);
						grew = true;
						temp_b = rand_b + 1;
					}
					
				}
				if (!grew)
				{
					iteration = false;
				}
				else
				{
					if (temp_a > 0 && temp_a < (tiles_per_grid/3) && temp_b > 0 && temp_b < (tiles_per_grid / 3))
					{
						//rand_x = temp_x;
						//rand_y = temp_y;
						rand_a = temp_a;
						rand_b = temp_b;
					}
					else
					{
						iteration = false;
					}
					
				}
				
			}
			std::cout << "DEBUG: Growth loop ended\n";

		}
	}
	for (int x = 0; x < number_of_cells; x++)
	{
		//std::uniform_int_distribution<> stone_chance(10, 4);
		for (int y = 0; y < number_of_cells; y++)
		{
			for (int a = 0; a < tiles_per_grid / 3; a++)
			{
				for (int b = 0; b < tiles_per_grid / 3; b++)
				{
					if (array_reference[x][y][a][b].get_tile_type() == 0)
					{
						if (a - 1 >= 0)
						{
							if (array_reference[x][y][a - 1][b].get_tile_type() == 2)
							{
								array_reference[x][y][a-1][b].set_tile_type(1);
							}
						}
						if (a + 1 < tiles_per_grid/3)
						{
							if (array_reference[x][y][a + 1][b].get_tile_type() == 2)
							{
								array_reference[x][y][a + 1][b].set_tile_type(1);
							}
						}
						if (b - 1 >= 0)
						{
							if (array_reference[x][y][a][b - 1].get_tile_type() == 2)
							{
								array_reference[x][y][a][b - 1].set_tile_type(1);
							}
						}
						if (b + 1 < tiles_per_grid / 3)
						{
							if (array_reference[x][y][a][b + 1].get_tile_type() == 2)
							{
								array_reference[x][y][a][b + 1].set_tile_type(1);
							}
						}
					}
				}
			}
		}
	}
}
void read_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3])
{
	io_file_data_read = "";
	io_file_data_line = "";
	io_tile_type_string = "";
	std::vector<int> io_tile_type_vector;
	std::string file_name = "world/" + std::to_string(world_coordinates.x) + ',' + std::to_string(world_coordinates.y) + '.' + "grid";

	std::cout << "DEBUG: File name read: " + file_name + " \n";
	std::ifstream io_file_stream( file_name , std::ios::in);
	std::cout << io_file_stream.is_open();
	std::cout << "DEBUG: Attempting to read file " << file_name << ".\n";
	bool read_failure = false;
	if (io_file_stream.is_open())
	{
		std::getline(io_file_stream, io_file_data_read);
		//std::cout << "DEBUG: File read = " << io_file_data_read << "\n";
		while (getline(io_file_stream, io_file_data_line))
		{
			//std::cout << "DEBUG:Reading file.\n";
			io_file_data_read += io_file_data_line;
		}
		//std::cout << "DEBUG:data = " << io_file_data_read << "\n";
		//std::cout << "DEBUG:Parsing file data.\n";
					for (int x = 0; x < io_file_data_read.length(); x++)
		{
			if (io_file_data_read[x] == ':')
			{
				//std::cout << "DEBUG:Colon detected.\n";
				io_tile_type_string = "";
				int y = 1;
				while (io_file_data_read[x + y] != ';')
				{
					io_tile_type_string += io_file_data_read[x + y];
					y++;
				}
				//std::cout << "DEBUG:Number read = " << io_tile_type_string<<".\n";

				x = (x + y);
				for (int s = 0; s < io_tile_type_string.size(); s++)
				{
					//if (!std::isdigit(io_tile_type_string[s]) || io_tile_type_string == "")
					//{

					//	read_failure = true;
					//}
					if (io_tile_type_string.empty() || !std::all_of(io_tile_type_string.begin(), io_tile_type_string.end(), [](char c) {return std::isdigit(c);}))
					{
						read_failure = true;
					}
				}
				if (!read_failure)
				{
					io_tile_type_vector.push_back(std::stoi(io_tile_type_string));
				}
			}
		}
		//std::cout << "DEBUG:Parsing complete.\n";

		int index = 0;
		for (int x = 0; x < number_of_cells; x++)
		{
			for (int y = 0; y < number_of_cells; y++)
			{
				for (int a = 0; a < tiles_per_grid / 3; a++)
				{
					for (int b = 0; b < tiles_per_grid / 3; b++)
					{
						//std::cout << "DEBUG:Number of tiles to set = " << io_tile_type_vector.size() << "\n";
						if (index < io_tile_type_vector.size())
						{
							array_reference[x][y][a][b].set_tile_type(io_tile_type_vector[index]);
							index++;
						}
						else
						{
							read_failure = true;
						}
					}
				}
			}
		}
		//std::cout << "DEBUG: tile setting complete, closing file stream.\n";
		if (io_file_stream.is_open())
		{
			io_file_stream.close();
		}
		if (read_failure)
		{
			std::cout << "World file corrupted. Generating new world file.\n";
			std::filesystem::remove(file_name);
			write_new_grid(world_coordinates, array_reference);
		}
		//std::cout << "DEBUG:File stream closed, setting textures.\n";

	}
	else
	{
		write_new_grid(world_coordinates, array_reference);
	}
}
void write_new_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3])
{
	//std::cout << "DEBUG:File not found, generating new tileset\n";
	std::ofstream io_writer("world/" + std::to_string(world_coordinates.x) + ',' + std::to_string(world_coordinates.y) + '.' + "grid");
	debug_randomize_tiles(array_reference);

	for (int x = 0; x < number_of_cells; x++)
	{
		for (int y = 0; y < number_of_cells; y++)
		{
			for (int a = 0; a < tiles_per_grid / 3; a++)
			{
				for (int b = 0; b < tiles_per_grid / 3; b++)
				{
					io_tile_type_string = ":" + std::to_string(array_reference[x][y][a][b].get_tile_type()) + ";";
					io_writer << io_tile_type_string;
				}
			}
		}
	}
	io_writer.close();
	//std::cout << "DEBUG: Textures set.\n";
}
bool peek_next_grid(sf::Vector2i attempted_world_coordinates, sf::Vector2i attempted_cell, sf::Vector2i attempted_grid_coordinates)
{
	read_grid(attempted_world_coordinates, temp_grid);
	return (temp_grid[attempted_cell.x][attempted_cell.y][attempted_grid_coordinates.x][attempted_grid_coordinates.y].get_passable());
}
void move_player(sf::Keyboard::Key movement_key_pressed, sf::Vector2i attempted_movement)
{
	bool world_coordinate_is_passable = true;
	projected_cell = current_cell;
	projected_world_coordinates = current_world_coordinates;
	projected_coordinates = { player.get_grid_position_x() + attempted_movement.x, player.get_grid_position_y() + attempted_movement.y };
	if (menu_is_open < 1)
	{

		if (projected_coordinates.x < 0)
		{
			projected_coordinates.x = (tiles_per_grid / 3) - 1;
			projected_cell.x--;
		}
		else if (projected_coordinates.x > (tiles_per_grid / 3) - 1)
		{
			projected_coordinates.x = 0;
			projected_cell.x++;
		}
		if (projected_coordinates.y < 0)
		{
			projected_coordinates.y = (tiles_per_grid / 3) - 1;
			projected_cell.y--;
		}
		else if (projected_coordinates.y > (tiles_per_grid / 3) - 1)
		{
			projected_coordinates.y = 0;
			projected_cell.y++;
		}
		if (projected_cell.x < 0)
		{
			projected_cell.x = number_of_cells - 1;
			projected_world_coordinates.x--;
		}
		else if (projected_cell.x > number_of_cells - 1)
		{
			projected_cell.x = 0;
			projected_world_coordinates.x++;
		}
		if (projected_cell.y < 0)
		{
			projected_cell.y = number_of_cells - 1;
			projected_world_coordinates.y--;
		}
		else if (projected_cell.y > number_of_cells - 1)
		{
			projected_cell.y = 0;
			projected_world_coordinates.y++;
		}
		if (projected_world_coordinates != current_world_coordinates)
		{
			world_coordinate_is_passable = peek_next_grid(projected_world_coordinates, projected_cell, projected_coordinates);
		}
		if ((timer - last_time) > 500)
		{
			last_time = timer - 450;
			key_pressed = true;
			if (tile_grid[projected_cell.x][projected_cell.y][projected_coordinates.x][projected_coordinates.y].get_passable() && world_coordinate_is_passable)
			{
				last_cell = current_cell;
				last_world_coordinates = current_world_coordinates;
				player.move(attempted_movement.x, attempted_movement.y, current_cell, current_world_coordinates);
				last_key_pressed = movement_key_pressed;
			}
		}
		else if (!key_pressed)
		{
			if (tile_grid[projected_cell.x][projected_cell.y][projected_coordinates.x][projected_coordinates.y].get_passable() && world_coordinate_is_passable)
			{
				player.move(attempted_movement.x, attempted_movement.y, current_cell, current_world_coordinates);
			}
			key_pressed = true;
			last_key_pressed = movement_key_pressed;
			last_time = static_cast<double>(current_time.count());
		}
	}
}
sf::Keyboard::Key get_key_pressed()
{
	key_pressed_storage = sf::Keyboard::Key::Unknown;
	while (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		//TODO: Add a less stupid way to guard against this.
	}
	while (key_pressed_storage == sf::Keyboard::Key::Unknown)
	{
		for (int x = 0; x < static_cast<int>(sf::Keyboard::ScancodeCount); x++)
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(static_cast<sf::Keyboard::Scan>(x))))
			{
				return sf::Keyboard::localize(static_cast<sf::Keyboard::Scan>(x));
			}

		}
	}
}