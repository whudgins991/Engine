#include "main.hpp"
#include "Tile.hpp"
#include "Player.hpp"
#include "Menu.hpp"


//The window
sf::RenderWindow window;
//Set the number of rows/columns in current grid. Chaning this will change the arrays/vectors further in. Note: Unexpected things may happen if this is not a multiple of 3
const int tiles_per_grid = 81;
//Set the number of rows/columns of cells (screens) per grid (loaded tile sets). This can also be changed
const int number_of_cells = 3;
//Set how large each tile appears on screen
sf::Vector2f render_scale;
Player player;
//Used for control logic
bool key_pressed = false;
std::chrono::milliseconds current_time;
double timer;
double last_time;
sf::Keyboard::Key last_changable_key_pressed;
sf::Keyboard::Key last_fixed_key_pressed;
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
sf::Texture water_texture("Sprites/water_sprite.png");
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
sf::Texture tree_base_texture("Sprites/tree_base_sprite.png");
sf::Texture tree_top_texture("Sprites/tree_top_sprite.png");
sf::Texture empty_texture("Sprites/empty_sprite.png");
//Used to denote which screen is cell is rendered at any given time and handle movement logic
sf::Vector2i projected_coordinates = { 0, 0 };
sf::Vector2i projected_cell = { 0, 0 };
sf::Vector2i projected_world_coordinates = { 0, 0 };
sf::Vector2i current_cell = { 0, 0 };
sf::Vector2i current_world_coordinates = { 0 , 0 };
sf::Vector2i last_cell = { 0, 0 };
sf::Vector2i last_world_coordinates = { 0, 0 };
//This is the 2d vector used to display the current cell
std::vector<std::vector<sf::Sprite>> sprite_vector;
//Used for the second layer of sprites. Think tree tops and such
std::vector<std::vector<sf::Sprite>> sprite_vector_layered;
//Strings for use in the I/O reader and writer
std::string io_file_data_read;
std::string io_file_data_line;
std::string io_tile_type_string;
//Random number generator
std::random_device random;
std::mt19937 random_generator(random());
//Vector of menus. 
std::vector<Menu> menu_vector;
//The int that determines which menu is open. 0 = no menu, 1 = the default pause menu, 2 = the options menu, etc
int menu_is_open = 0;
//Used for setting keybinds
sf::Keyboard::Key key_pressed_storage;
//Used for fullscreen toggling
bool is_fullscreen = true;
bool was_fullscreen = true;
//Default, unchangable controls. Note that the scancode (numeric representation of the key pressed) is used instead of the key. This makes the keybind logic slightly easier to implement.
static std::array<sf::Keyboard::Scancode, 10> fixed_keybind_array = { sf::Keyboard::Scancode::Space, sf::Keyboard::Scancode::Numpad1, sf::Keyboard::Scancode::Numpad2, sf::Keyboard::Scancode::Numpad3, sf::Keyboard::Scancode::Numpad4, sf::Keyboard::Scancode::Numpad5, sf::Keyboard::Scancode::Numpad6, sf::Keyboard::Scancode::Numpad7, sf::Keyboard::Scancode::Numpad8, sf::Keyboard::Scancode::Numpad9 };
//Rebindable controls
std::array<sf::Keyboard::Scancode, 10> changable_keybind_array = { sf::Keyboard::Scancode::Enter, sf::Keyboard::Scancode::Z, sf::Keyboard::Scancode::S, sf::Keyboard::Scancode::C, sf::Keyboard::Scancode::A, sf::Keyboard::Scancode::X, sf::Keyboard::Scancode::D, sf::Keyboard::Scancode::Q, sf::Keyboard::Scancode::W, sf::Keyboard::Scancode::E };

//used to realign the sprites on the window after toggling fullscreen
void realign();
//Sets the tile trextures based on tile type
void set_tile_textures();
//World generation logic for new grids
void randomize_tiles(Tile (&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3]);
//Function to read a grid from a file
void read_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3]);
//Function to generate and then write a new grid to a file
void write_new_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3]);
//A slight modification to the read_grid function - this peeks into a file for the sake fo collision logic
bool peek_next_grid(sf::Vector2i attempted_world_coordinates, sf::Vector2i attempted_cell, sf::Vector2i attempted_grid_coordinates);
//Player movement logic
void move_player(sf::Keyboard::Key changable_key_pressed, sf::Keyboard::Key fixed_key_pressed, sf::Vector2i attempted_movement);
//Function to set new keybinds
void set_keybind(sf::Keyboard::Scancode& changed_key);

int main()
{
    // create the window
    window.create(sf::VideoMode({ 160, 160 }), "My window", sf::Style::Default, sf::State::Fullscreen);

	//Calculate the empty space and render scale
	empty_space = ((static_cast<float>(window.getSize().x) - static_cast<float>(window.getSize().y)) / 2);
	render_scale = { ((static_cast<float>(window.getSize().x) - (empty_space * 2)) / (tiles_per_grid/3)), (static_cast<float>(window.getSize().y) / (tiles_per_grid/3)) };

	//Fill the menu vector
	for (int x = 0; x < 4; x++)
	{
		Menu m;
		m.set_menu_index(x);
		menu_vector.push_back(m);
	}
	//Add the menu text. Note: menu_vector[0] is a dummy menu that will never be rendered. I originally had the pause menu as [0] but having the menu index not match menu_is_open got annoying
	menu_vector[0].set_menu_text({ "" });
	menu_vector[1].set_menu_text({ "Resume", "Options", "Exit Game", "DEBUG: WIPE WORLD"});
	menu_vector[2].set_menu_text({ "Toggle Fullscreen", "Keybinds", "Back" });
	menu_vector[3].set_menu_text({ "Move Down/Left = "+ sf::Keyboard::getDescription(fixed_keybind_array[1]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[1]).toAnsiString(), "Move Down = " + sf::Keyboard::getDescription(fixed_keybind_array[2]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[2]).toAnsiString(), "Move Down/Right = " + sf::Keyboard::getDescription(fixed_keybind_array[3]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[3]).toAnsiString(), "Move Left = " + sf::Keyboard::getDescription(fixed_keybind_array[4]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[4]).toAnsiString(), "Stand Still = " + sf::Keyboard::getDescription(fixed_keybind_array[5]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[5]).toAnsiString(), "Move Right = " + sf::Keyboard::getDescription(fixed_keybind_array[6]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[6]).toAnsiString(), "Move Up/Left = " + sf::Keyboard::getDescription(fixed_keybind_array[7]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[7]).toAnsiString(), "Move Up = " + sf::Keyboard::getDescription(fixed_keybind_array[8]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[8]).toAnsiString(), "Move Up/Right = " + sf::Keyboard::getDescription(fixed_keybind_array[9]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[9]).toAnsiString(), "Select = " + sf::Keyboard::getDescription(fixed_keybind_array[0]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[0]).toAnsiString(), "DEFAULT",  "Back"});
	for (int x = 0; x < menu_vector.size(); x++)
	{
		menu_vector[x].set_fill_color(sf::Color(0, 0, 0, 128));
		menu_vector[x].set_outline_color(sf::Color::Magenta);
		menu_vector[x].set_scale(render_scale);
		menu_vector[x].set_position({ static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2  });
	}
	
	//Passes the player class all info needed
	player.set_spacing(empty_space, (tiles_per_grid/3),number_of_cells, render_scale);


	//Fills in the vectors used for sprite rendering
	for (int x = 0; x < tiles_per_grid / 3; x++)
	{
		std::vector<sf::Sprite> v;
		sprite_vector.push_back(v);
		sprite_vector_layered.push_back(v);
		for (int y = 0; y < tiles_per_grid / 3; y++)
		{
			sprite_vector[x].push_back(sf::Sprite(test_tile_texture));
			sprite_vector_layered[x].push_back(sf::Sprite(test_tile_texture));

		}
	}

	//Read the starting grif from a file
	read_grid(current_world_coordinates, tile_grid);
	//Set the sprite vectors to the tiles of the current cell
	set_tile_textures();

    //Run the program as long as the window is open
	while (window.isOpen())
    {
		//If the program loses focus, do nothing
		while (!window.hasFocus())
		{

		}

		//Used to determine how long a button has been held for movement purposes
		current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		timer = static_cast<double>(current_time.count());


        //Check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent())
        {
            //"Close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
			if (event->is<sf::Event::Resized>())
				realign();
        }
		//Used to determine if the cell has changed for drawing logic
		last_cell = current_cell;
		last_world_coordinates = current_world_coordinates;
		//Used to determine if a key is being held
		if (!sf::Keyboard::isKeyPressed(last_changable_key_pressed) && !sf::Keyboard::isKeyPressed(last_fixed_key_pressed))
		{
			key_pressed = false;
		}
		//If no movement keys are held down, reset the timer
		if (!key_pressed)
		{
			timer = 0;
		}
		//Escape key handling
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && !key_pressed)
		{
			last_fixed_key_pressed = sf::Keyboard::Key::Escape;
			last_changable_key_pressed = sf::Keyboard::Key::Escape;	
			key_pressed = true;
			//If the menu is open, go back one menu, or close it entirely if on the first menu
			if (menu_is_open > 0)
			{
				menu_is_open--;
				menu_vector[menu_is_open].menu_change();
			}
			//Else, open the menu
			else
			{
				menu_is_open = 1;
				menu_vector[menu_is_open].menu_change();

			}
			//Prevent the menu index from going below zero
			if (menu_is_open < 0)
			{
				menu_is_open = 0;
			}
		}
		//Handling of movement keys while in the menu
		if (menu_is_open && (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[8])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[8]))) && !key_pressed)
		{
			last_changable_key_pressed = sf::Keyboard::localize(changable_keybind_array[8]);
			last_fixed_key_pressed = sf::Keyboard::localize(fixed_keybind_array[8]);
			key_pressed = true;
			menu_vector[menu_is_open].move_up();
		}
		if (menu_is_open && (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[2])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[2]))) && !key_pressed)
		{
			last_changable_key_pressed = sf::Keyboard::localize(changable_keybind_array[2]);
			last_fixed_key_pressed = sf::Keyboard::localize(fixed_keybind_array[2]);
			key_pressed = true;
			menu_vector[menu_is_open].move_down();
		}
		//Handling of movement keys outside of menus
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[1])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[1])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[1]), sf::Keyboard::localize(fixed_keybind_array[1]), { -1, 1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[2])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[2])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[2]), sf::Keyboard::localize(fixed_keybind_array[2]), { 0, 1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[3])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[3])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[3]), sf::Keyboard::localize(fixed_keybind_array[3]), { 1, 1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[4])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[4])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[4]), sf::Keyboard::localize(fixed_keybind_array[4]), { -1, 0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[5])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[5])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[5]), sf::Keyboard::localize(fixed_keybind_array[5]), { 0, 0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[6])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[6])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[6]), sf::Keyboard::localize(fixed_keybind_array[6]), { 1, 0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[7])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[7])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[7]), sf::Keyboard::localize(fixed_keybind_array[7]), { -1 , -1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[8])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[8])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[8]), sf::Keyboard::localize(fixed_keybind_array[8]), { 0, -1 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[9])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[9])))
		{
			move_player(sf::Keyboard::localize(changable_keybind_array[9]), sf::Keyboard::localize(fixed_keybind_array[9]), { 1 , -1 });
		}
		//Handling of the selection key while the menu is open
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::localize(changable_keybind_array[0])) || sf::Keyboard::isKeyPressed(sf::Keyboard::localize(fixed_keybind_array[0]))) && !key_pressed && menu_is_open > 0)
		{
			last_changable_key_pressed = sf::Keyboard::localize(changable_keybind_array[0]);
			last_fixed_key_pressed = sf::Keyboard::localize(fixed_keybind_array[0]);
			key_pressed = true;
			//This is the logic for selecting menu options. menu_vector[x].select() returns a code used to determine what menu option was selected and what action should be taken.
			switch (menu_vector[menu_is_open].select())
			{
			//This is a debug option used for completely wiping the currently saved world. Do not select this option if you care about currently generated grids
			case -99:
			{
				//Just straight up remove and recreate the world generation folder then close the game
				std::filesystem::remove_all("World/");
				std::filesystem::create_directory("World/");
				window.close();
				break;
			}
			//This options resets the keybinds to their defaults
			case -3:
				changable_keybind_array[1] = sf::Keyboard::Scancode::Z;
				changable_keybind_array[2] = sf::Keyboard::Scancode::S;
				changable_keybind_array[3] = sf::Keyboard::Scancode::C;
				changable_keybind_array[4] = sf::Keyboard::Scancode::A;
				changable_keybind_array[5] = sf::Keyboard::Scancode::X;
				changable_keybind_array[6] = sf::Keyboard::Scancode::D;
				changable_keybind_array[7] = sf::Keyboard::Scancode::Q;
				changable_keybind_array[8] = sf::Keyboard::Scancode::W;
				changable_keybind_array[9] = sf::Keyboard::Scancode::E;
				changable_keybind_array[0] = sf::Keyboard::Scancode::Enter;
				menu_vector[3].clear_menu_text();
				//Reset the menu text displaying the current keybinds
				menu_vector[3].set_menu_text({ "Move Down/Left = " + sf::Keyboard::getDescription(fixed_keybind_array[1]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[1]).toAnsiString(), "Move Down = " + sf::Keyboard::getDescription(fixed_keybind_array[2]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[2]).toAnsiString(), "Move Down/Right = " + sf::Keyboard::getDescription(fixed_keybind_array[3]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[3]).toAnsiString(), "Move Left = " + sf::Keyboard::getDescription(fixed_keybind_array[4]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[4]).toAnsiString(), "Stand Still = " + sf::Keyboard::getDescription(fixed_keybind_array[5]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[5]).toAnsiString(), "Move Right = " + sf::Keyboard::getDescription(fixed_keybind_array[6]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[6]).toAnsiString(), "Move Up/Left = " + sf::Keyboard::getDescription(fixed_keybind_array[7]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[7]).toAnsiString(), "Move Up = " + sf::Keyboard::getDescription(fixed_keybind_array[8]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[8]).toAnsiString(), "Move Up/Right = " + sf::Keyboard::getDescription(fixed_keybind_array[9]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[9]).toAnsiString(), "Select = " + sf::Keyboard::getDescription(fixed_keybind_array[0]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[0]).toAnsiString(), "DEFAULT",  "Back" });
				menu_vector[3].set_scale(render_scale);
				menu_vector[3].set_outline_color(sf::Color::Magenta);
				menu_vector[3].set_position({ static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2 });
				break;
			//This option toggles fullscreen mode. TODO: Implement logic for changing resolution without having to manually resize the window
			case -2:
				is_fullscreen = !is_fullscreen;
				if (is_fullscreen)
				{
					window.create(sf::VideoMode({ 160, 160 }), "My window", sf::Style::Default, sf::State::Fullscreen);
				}
				else
				{
					window.create(sf::VideoMode({ 2000, 1600 }), "My window", sf::Style::Default, sf::State::Windowed);
				}
				break;
			//This option closes the window
			case -1:
				window.close();
				break;
			//Switch to menu_vector[0], which closes the menu
			case 0:
				menu_is_open = 0;
				break;
			//Menu_vector[1] - the default pause menu
			case 1:
				menu_is_open = 1;
				break;
			//[2], the options menu
			case 2:
				menu_is_open = 2;
				break;
			//[3], the keybinds screen
			case 3:
				menu_is_open = 3;
				break;
			//This case is for rebinding the select key
			case 90:
				set_keybind(changable_keybind_array[0]);
				break;
			//Rebind the movement keys. Think of a joystick sitting on numpad 5. Each number sans 5 moves in that direction. This one rebinds the key to move down and to the elft
			case 91:
				set_keybind(changable_keybind_array[1]);
				break;
			//Rebind down movement
			case 92:
				set_keybind(changable_keybind_array[2]);
				break;
			//Down, right
			case 93:
				set_keybind(changable_keybind_array[3]);
				break;
			//Left
			case 94:
				set_keybind(changable_keybind_array[4]);
				break;
			case 95:
			//5 does not move, but instead waits in place for a turn, allowing everything else in the world to move without moving the player
				set_keybind(changable_keybind_array[5]);
				break;
			//Move right
			case 96:
				set_keybind(changable_keybind_array[6]);
				break;
			//Up, left
			case 97:
				set_keybind(changable_keybind_array[7]);
				break;
			//Up
			case 98:
				set_keybind(changable_keybind_array[8]);
				break;
			//Up, right
			case 99:
				set_keybind(changable_keybind_array[9]);
				break;
			default:
				break;
			}
		}
		//If the fullscreen status is changed, realign the sprites
		if (was_fullscreen != is_fullscreen)
		{
			realign();
		}
	
        //Clear the window with black color, otherwise each frame just renders on top of the previous one
        window.clear(sf::Color::Black);

        //Drawing logic follows
		//If the current cell has changed, update all onscreen sprites based on the new tiles on screen
		if (!(last_cell == current_cell))
		{
			set_tile_textures();
		}
		//If the current grid has changed, read it from a file and update the sprites accordingly
		if (!(last_world_coordinates == current_world_coordinates))
		{
			read_grid(current_world_coordinates, tile_grid);
			set_tile_textures();
		}
		//Draw all onscreen tile sprites
		for (int x = 0; x < tiles_per_grid / 3; x++)
		{
			for (int y = 0; y < tiles_per_grid / 3; y++)
			{
				window.draw(sprite_vector[x][y]);
			}
		}

		//Draw the player
		player.draw(window);
		for (int x = 0; x < tiles_per_grid / 3; x++)
		{
			for (int y = 0; y < tiles_per_grid / 3; y++)
			{
				window.draw(sprite_vector_layered[x][y]);
			}
		}
		if (menu_is_open > 0)
		{
			menu_vector[menu_is_open].draw(window);
		}

        //End the current frame
        window.display();
    }
}

//See function declarations for the purpose of each function.
void realign()
{
	//Recalculate the important cariables
	empty_space = ((static_cast<float>(window.getSize().x) - static_cast<float>(window.getSize().y)) / 2);
	render_scale = { ((static_cast<float>(window.getSize().x) - (empty_space * 2)) / (tiles_per_grid / 3)), (static_cast<float>(window.getSize().y) / (tiles_per_grid / 3)) };
	//Pass the player the new calculations
	player.set_spacing(empty_space, (tiles_per_grid / 3), number_of_cells, render_scale);
	was_fullscreen = is_fullscreen;
	//Adjust tile sprite positions and scale
	for (int x = 0; x < tiles_per_grid / 3; x++)
	{
		for (int y = 0; y < tiles_per_grid / 3; y++)
		{
			sprite_vector[x][y].setScale({ render_scale.x / 16, render_scale.y / 16 });
			sprite_vector[x][y].setPosition({ ((render_scale.x * x) + empty_space), render_scale.y * y });
			sprite_vector_layered[x][y].setScale({ render_scale.x / 16, render_scale.y / 16 });
			sprite_vector_layered[x][y].setPosition({ ((render_scale.x * x) + empty_space), render_scale.y * y });
		}
	}
	//Adjust the menus
	for (int x = 0; x < menu_vector.size(); x++)
	{
		menu_vector[x].set_scale(render_scale);
		menu_vector[x].set_position({ static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2 });
	}
}
void set_tile_textures()
{

	for (int x = 0; x < tiles_per_grid / 3; x++)
	{
		for (int y = 0; y < tiles_per_grid / 3; y++)
		{	
			//Set the position, scale, and texture of each sprite based on the corresponding tile
			sprite_vector[x][y].setPosition({ ((render_scale.x * x) + empty_space), render_scale.y * y });
			sprite_vector[x][y].setScale({ render_scale.x / 16, render_scale.y / 16 });
			sprite_vector_layered[x][y].setPosition({ ((render_scale.x * x) + empty_space), render_scale.y * y });
			sprite_vector_layered[x][y].setScale({ render_scale.x / 16, render_scale.y / 16 });

			switch (tile_grid[current_cell.x][current_cell.y][x][y].get_tile_type())
			{
			//Note: texture_zero through texture_nine are debug textures that are not for use in the final product. test_tile_texture is the default texture set before any tiles are read and will only ever appear in error cases.
			case 0: sprite_vector[x][y].setTexture(stone_texture); tile_grid[current_cell.x][current_cell.y][x][y].set_passable(false); break;
			case 1: sprite_vector[x][y].setTexture(dirt_texture);tile_grid[current_cell.x][current_cell.y][x][y].set_passable(true); break;
			case 2: sprite_vector[x][y].setTexture(grass_texture);tile_grid[current_cell.x][current_cell.y][x][y].set_passable(true); break;
			case 3: sprite_vector[x][y].setTexture(tree_base_texture);tile_grid[current_cell.x][current_cell.y][x][y].set_passable(false); break;
			case 4: sprite_vector[x][y].setTexture(water_texture);tile_grid[current_cell.x][current_cell.y][x][y].set_passable(true); break;
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
			//Drawing logic for the second layer of sprites. TODO: Turn into a switch statement as more multi-level sprites are added
			if (tile_grid[current_cell.x][current_cell.y][x][y].get_tile_type() == 3 && y > 0)
			{
				sprite_vector_layered[x][y-1].setTexture(tree_top_texture);
			}
			//This is a blank texture used for cases where a tile does not have a second layer.
			sprite_vector_layered[x][y].setTexture(empty_texture);

		}
	}

}
void randomize_tiles(Tile (&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3])
{
	//Storage for randomly generated numbers. Note: this list will likely grow or shrink as I experiment with different methods of world generation
	int rand_x;
	int rand_y;
	int rand_a;
	int rand_b;
	int temp_a;
	int temp_b;
	//Random distributions used for world generation logic
	std::uniform_int_distribution<> random_cells(0, number_of_cells - 1);
	std::uniform_int_distribution<> random_tiles(0, (tiles_per_grid / 3) - 1);
	std::uniform_int_distribution<> stone_chance(50, 500);
	std::uniform_int_distribution<> tree_chance(500, 5000);
	std::uniform_int_distribution<> lake_chance(0, 40);
	std::uniform_int_distribution<> random_chance_to_continue_stone(0, 1);
	std::uniform_int_distribution<> random_chance_to_continue_lake(0, 2);
	std::uniform_int_distribution<> random_growth_points_deduction(0, 5);
	//Storage for the coordinates of the tile that is currently growing
	sf::Vector2i growth_coordinates_storage;
	std::vector<sf::Vector2i> growth_vector;
	//Numbers used during growth logic for different tile types, namely stone and water at the moment.
	int growth_points;
	int chance_to_continue;
	bool iteration;
	bool grew = false;

	//First pass, set everything to grass
	for (int x = 0; x < number_of_cells; x++)
	{
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
	//Second pass, place random stone outcroppings
	for (int i = 0;i < (stone_chance(random_generator)); i++)
	{
		//Select random spots for the stone to start
		rand_x = random_cells(random_generator);
		rand_y = random_cells(random_generator);
		rand_a = random_tiles(random_generator);
		rand_b = random_tiles(random_generator);
		//Never generate on the player's current position
		if (!((rand_x == current_cell.x) && (rand_y == current_cell.y) &&(rand_a == player.get_grid_position_x()) &&(rand_b == player.get_grid_position_y())))
		{
			array_reference[rand_x][rand_y][rand_a][rand_b].set_tile_type(0);
			//Continue to grow while growth conditions are still met
			iteration = true;
			while (iteration == true)
			{
				grew = false;
				temp_a = rand_a;
				temp_b = rand_b;
				chance_to_continue = random_chance_to_continue_stone(random_generator);
				//If chance_to_continue randomly lands on 0, grow. TODO: Replace this with the growth points system
				if (chance_to_continue == 0)
				{
					if (rand_a > 0 && array_reference[rand_x][rand_y][rand_a - 1][rand_b].get_tile_type() == 2)
					{
						array_reference[rand_x][rand_y][rand_a - 1][rand_b].set_tile_type(0);
						grew = true;
						temp_a = rand_a - 1;
						

					}
					if (rand_a < ((tiles_per_grid / 3) - 1) && (array_reference[rand_x][rand_y][rand_a + 1][rand_b].get_tile_type() == 2))
					{
						array_reference[rand_x][rand_y][rand_a + 1][rand_b].set_tile_type(0);
						grew = true;
						temp_a = rand_a + 1;
					}
					if (rand_b > 0 && array_reference[rand_x][rand_y][rand_a][rand_b - 1].get_tile_type() == 2)
					{
						array_reference[rand_x][rand_y][rand_a][rand_b - 1].set_tile_type(0);
						grew = true;
						temp_b = rand_b - 1;
						

					}
					if (rand_b < (tiles_per_grid / 3) - 1 && array_reference[rand_x][rand_y][rand_a][rand_b + 1].get_tile_type() == 2)
					{
						array_reference[rand_x][rand_y][rand_a][rand_b + 1].set_tile_type(0);
						grew = true;
						temp_b = rand_b + 1;
					}
				}
				//If no growth occurred, end the growth loop. This was implemented as a random way to cap growing
				if (!grew)
				{
					iteration = false;
				}
				else
				{
					if (temp_a > 0 && temp_a < (tiles_per_grid/3) && temp_b > 0 && temp_b < (tiles_per_grid / 3))
					{
						rand_a = temp_a;
						rand_b = temp_b;
					}
					else
					{
						iteration = false;
					}
					
				}
				
			}

		}
	}
	
	//Third pass, place random lakes. Note: lakes are different from rivers in that lakes grow outward randomly and rivers will tend to grow in a single direction. TODO: Implement rivers
	for (int i = 0;i < (lake_chance(random_generator)); i++)
	{

		rand_x = random_cells(random_generator);
		rand_y = random_cells(random_generator);
		rand_a = random_tiles(random_generator);
		rand_b = random_tiles(random_generator);
		// Growth points are a different system I'm experimenting with to allow a cluster of tiles to grow greatly within certain bounds. This allows me to set the growth rate much higher without running into the possibility of it growing out of control and overtaking the whole cell. Once growth points hit zero, growth stops.
		growth_points = 250;
		//Once again, never generate on top of the player
		if (!((rand_x == current_cell.x) && (rand_y == current_cell.y) && (rand_a == player.get_grid_position_x()) && (rand_b == player.get_grid_position_y()))) 
		{
			array_reference[rand_x][rand_y][rand_a][rand_b].set_tile_type(4);
			//The idea here is to start growth on a single tile (growth_vector[0]) and then add the coordinates of every tile that grew from there. Tiles are then popped off the vector as they themselves grow
			growth_vector.push_back(sf::Vector2i(rand_a, rand_b));

			while (!growth_vector.empty() && growth_points > 0)
			{

				chance_to_continue = random_chance_to_continue_lake(random_generator);
				growth_coordinates_storage = (growth_vector.back());
				growth_vector.pop_back();
				//Unlike with stone, a new random number is generated to determine growth in every individual direction, instead of all directions growing at once. This is more costly, but creates less predictable, more organic growth patterns
				if (random_chance_to_continue_lake(random_generator) == 0 && growth_coordinates_storage.x > 0 && growth_coordinates_storage.y > 0)
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x - 1][growth_coordinates_storage.y - 1].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x - 1, growth_coordinates_storage.y - 1 });
					growth_points--;
				}
				if (random_chance_to_continue_lake(random_generator) == 0 && growth_coordinates_storage.y > 0)
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x][growth_coordinates_storage.y - 1].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x, growth_coordinates_storage.y - 1 });
					growth_points--;
				}
				if (random_chance_to_continue_lake(random_generator) == 0 && growth_coordinates_storage.x < (tiles_per_grid - 1) && growth_coordinates_storage.y > 0)
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x + 1][growth_coordinates_storage.y - 1].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x + 1, growth_coordinates_storage.y - 1 });
					growth_points--;
				}
				if (random_chance_to_continue_lake(random_generator) == 0 && growth_coordinates_storage.x > 0 )
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x - 1][growth_coordinates_storage.y].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x - 1, growth_coordinates_storage.y });
					growth_points--;
				}
				if (random_chance_to_continue_lake(random_generator) == 0 && growth_coordinates_storage.x < (tiles_per_grid - 1) && growth_coordinates_storage.y > 0)
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x + 1][growth_coordinates_storage.y - 1].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x + 1, growth_coordinates_storage.y - 1 });
					growth_points--;
				}
				if (random_chance_to_continue_lake(random_generator) == 0 && growth_coordinates_storage.x > 0 && growth_coordinates_storage.y < (tiles_per_grid - 1))
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x - 1][growth_coordinates_storage.y + 1].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x - 1, growth_coordinates_storage.y + 1 });
					growth_points--;
				}
				if (random_chance_to_continue_lake(random_generator) == 0  && growth_coordinates_storage.y < (tiles_per_grid - 1))
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x][growth_coordinates_storage.y + 1].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x, growth_coordinates_storage.y + 1 });
					growth_points--;
				}
				
				if (random_chance_to_continue_lake(random_generator) == 0 && growth_coordinates_storage.x > 0 && growth_coordinates_storage.y > 0)
				{
					array_reference[rand_x][rand_y][growth_coordinates_storage.x - 1][growth_coordinates_storage.y - 1].set_tile_type(4);
					growth_vector.push_back({ growth_coordinates_storage.x - 1, growth_coordinates_storage.y - 1 });
					growth_points--;	
				}
				growth_points -= random_growth_points_deduction(random_generator);
			}
			//Clear the growth_vector to ensure that it is empty for the next lake generation
			growth_vector.clear();
		}

	}
	//Logic for randomly placing trees. Note: trees are simply placed individually instead of growing in clusters
	for (int i = 0;i < (tree_chance(random_generator)); i++)
	{

		rand_x = random_cells(random_generator);
		rand_y = random_cells(random_generator);
		rand_a = random_tiles(random_generator);
		rand_b = random_tiles(random_generator);

		if (!((rand_x == current_cell.x) && (rand_y == current_cell.y) && (rand_a == player.get_grid_position_x()) && (rand_b == player.get_grid_position_y())))
		{
			array_reference[rand_x][rand_y][rand_a][rand_b].set_tile_type(3);
		}
	}
	//This is the logic for placing the tree tops. Tree tops are rendered over the player but can be moved through, simulating depth
	for (int x = 0; x < number_of_cells; x++)
	{
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

//This function takes in the world coordinates (grid) to be read and reads them into the array reference passed. This allows grids to be read without being rendered (for collision logic)
void read_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3])
{
	io_file_data_read = "";
	io_file_data_line = "";
	io_tile_type_string = "";
	std::vector<int> io_tile_type_vector;
	//My file naming convention for my grid files is simply the world coordinates of the grid (separated by a comma) with the extension ".grid"
	std::string file_name = "world/" + std::to_string(world_coordinates.x) + ',' + std::to_string(world_coordinates.y) + '.' + "grid";
	std::ifstream io_file_stream( file_name , std::ios::in);
	bool read_failure = false;
	//If the file as successfully opened...
	if (io_file_stream.is_open())
	{
		//Create a string with the raw data
		std::getline(io_file_stream, io_file_data_read);
		while (getline(io_file_stream, io_file_data_line))
		{
			io_file_data_read += io_file_data_line;
		}
		//Parse the data into tile types
		for (int x = 0; x < io_file_data_read.length(); x++)
		{
			//My current file format is a colon, followed by the numberic tile type, followed by a semicolon. This segment of code looks for the starting colon and reads in the string until it reaches the semicolon
			if (io_file_data_read[x] == ':')
			{
				io_tile_type_string = "";
				int y = 1;
				while (io_file_data_read[x + y] != ';')
				{
					io_tile_type_string += io_file_data_read[x + y];
					y++;
				}
				x = (x + y);
				for (int s = 0; s < io_tile_type_string.size(); s++)
				{
					//If the numeric string is empty, or if any characters within it are not numbers, the grid file is considered corrupted and reading fails
					if (io_tile_type_string.empty() || !std::all_of(io_tile_type_string.begin(), io_tile_type_string.end(), [](char c) {return std::isdigit(c);}))
					{
						read_failure = true;
					}
				}
				//If reading succeeded, convert the numeric string to an int and push it to the temporary vector
				if (!read_failure)
				{
					io_tile_type_vector.push_back(std::stoi(io_tile_type_string));
				}
			}
		}
		//Transfar the contents of the temporary vector to the 4D array used for holding the tile data
		int index = 0;
		for (int x = 0; x < number_of_cells; x++)
		{
			for (int y = 0; y < number_of_cells; y++)
			{
				for (int a = 0; a < tiles_per_grid / 3; a++)
				{
					for (int b = 0; b < tiles_per_grid / 3; b++)
					{
						if (index < io_tile_type_vector.size())
						{
							array_reference[x][y][a][b].set_tile_type(io_tile_type_vector[index]);
							index++;
						}
						//If somehow the code was not able to read enough tiles to fill the entire grid, the file is considered corrupted and reading fails
						else
						{
							read_failure = true;
						}
					}
				}
			}
		}
		//Close the file stream
		if (io_file_stream.is_open())
		{
			io_file_stream.close();
		}
		//If reading failed for any reason, delete the corrupted file and generate a new one
		if (read_failure)
		{
			std::cout << "DEBUG: World file corrupted. Generating new world file.\n";
			std::filesystem::remove(file_name);
			write_new_grid(world_coordinates, array_reference);
		}
	}
	//If the file could not be successfully opened, generate a new grid file
	else
	{
		write_new_grid(world_coordinates, array_reference);
	}
}
//This function also accepts an array reference. This is for cases where a new grid file must be generated, but you do not wish to display it yet
void write_new_grid(sf::Vector2i world_coordinates, Tile(&array_reference)[number_of_cells][number_of_cells][tiles_per_grid / 3][tiles_per_grid / 3])
{
	//Create the new .grid file
	std::ofstream io_writer("world/" + std::to_string(world_coordinates.x) + ',' + std::to_string(world_coordinates.y) + '.' + "grid");
	//Generate a new grid and store it in the array reference
	randomize_tiles(array_reference);

	for (int x = 0; x < number_of_cells; x++)
	{
		for (int y = 0; y < number_of_cells; y++)
		{
			for (int a = 0; a < tiles_per_grid / 3; a++)
			{
				for (int b = 0; b < tiles_per_grid / 3; b++)
				{
					//Create the string representing the tile type. TODO:: Add a more complex format to allow for tile health, movement penalties, etc
					io_tile_type_string = ":" + std::to_string(array_reference[x][y][a][b].get_tile_type()) + ";";
					io_writer << io_tile_type_string;
				}
			}
		}
	}
	io_writer.close();
}
//This returns a bool indicating if the player can move over their projected tile in the next grid. TODO: Rename to indicate this is related to passability?
bool peek_next_grid(sf::Vector2i attempted_world_coordinates, sf::Vector2i attempted_cell, sf::Vector2i attempted_grid_coordinates)
{
	read_grid(attempted_world_coordinates, temp_grid);
	return (temp_grid[attempted_cell.x][attempted_cell.y][attempted_grid_coordinates.x][attempted_grid_coordinates.y].get_passable());
}
//Player movement logic, namely logic for moving from cell to cell and grid to grid
void move_player(sf::Keyboard::Key changable_key_pressed, sf::Keyboard::Key fixed_key_pressed, sf::Vector2i attempted_movement)
{
	bool world_coordinate_is_passable = true;
	projected_cell = current_cell;
	projected_world_coordinates = current_world_coordinates;
	projected_coordinates = { player.get_grid_position_x() + attempted_movement.x, player.get_grid_position_y() + attempted_movement.y };
	//If the game is not paused...
	if (menu_is_open < 1)
	{
		//If the player moves beyond the bounds of one cell/grid, loop to the other side of the screen and set the projected path to the new cell/grid
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
		//Logic for holding down the movement keys. If the key is held for 500ms, move continuously
		if ((timer - last_time) > 500)
		{
			//Modify the timer so that the player moves every 50ms while the button is held
			last_time = timer - 450;
			key_pressed = true;
			if (tile_grid[projected_cell.x][projected_cell.y][projected_coordinates.x][projected_coordinates.y].get_passable() && world_coordinate_is_passable)
			{
				last_cell = current_cell;
				last_world_coordinates = current_world_coordinates;
				player.move(attempted_movement.x, attempted_movement.y, current_cell, current_world_coordinates);
				last_changable_key_pressed = changable_key_pressed;
				last_fixed_key_pressed = fixed_key_pressed;
			}
		}
		//Logic for tapping a movement key
		else if (!key_pressed)
		{
			if (tile_grid[projected_cell.x][projected_cell.y][projected_coordinates.x][projected_coordinates.y].get_passable() && world_coordinate_is_passable)
			{
				player.move(attempted_movement.x, attempted_movement.y, current_cell, current_world_coordinates);
			}
			key_pressed = true;
			last_changable_key_pressed = changable_key_pressed;
			last_fixed_key_pressed = fixed_key_pressed;
			last_time = static_cast<double>(current_time.count());
		}
	}
}
//Set custom keybinds. Take sin a reference to a scancode, waits for the user to press a key, then changes the referenced scancode to the code of the key pressed
void set_keybind(sf::Keyboard::Scancode &changed_key)
{
	bool change_failed = false;
	//While waiting for a button to be pressed, display a message
	//TODO: Rewrite to a seperate menu and allow the program to progress while waiting for input? Drawing within a fuction feels awkward
	menu_vector[3].clear_menu_text();
	menu_vector[3].set_menu_text({"WAITING FOR INPUT..."});
	menu_vector[3].set_scale(render_scale);
	menu_vector[3].set_outline_color(sf::Color::Red);
	menu_vector[3].set_position({ static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2 });
	menu_vector[3].draw(window);
	window.display();
	key_pressed_storage = sf::Keyboard::Key::Unknown;
	//If a key is being held as the keybind is first attempting to be set, wait for the key to be released before continuing. Otherwise it just sets the new keybind to the selection key frame 1 with no real chance to press the correct key
	while (key_pressed == true)
	{
		if (!sf::Keyboard::isKeyPressed(last_changable_key_pressed) && !sf::Keyboard::isKeyPressed(last_fixed_key_pressed))
		{
			key_pressed = false;
		}
	}
	//Loop until wither a new key is set or the change fails for whatever reason
	while (key_pressed_storage == sf::Keyboard::Key::Unknown && !change_failed)
	{
		//This is why scancodes were used for keybinds instead of keys. The scancodes can be looped through easily and converted to keys. This loops checks all possibly keys one by one to see if they are being pressed, then stores that key for use later
		for (int x = 0; x < static_cast<int>(sf::Keyboard::ScancodeCount); x++)
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::localize(static_cast<sf::Keyboard::Scancode>(x))))
			{
				key_pressed_storage = sf::Keyboard::localize(static_cast<sf::Keyboard::Scancode>(x));
				//If the key pressed is already in use, the change fails
				for (int y = 0; y < changable_keybind_array.size(); y++)
				{
					if (key_pressed_storage == sf::Keyboard::localize(changable_keybind_array[y]))
					{
						change_failed = true;
					}
				}
				//If the key pressed is part of the fixed, unchangeable keybinds, the change fails
				for (int y = 0; y < fixed_keybind_array.size(); y++)
				{
					if (key_pressed_storage == sf::Keyboard::localize(fixed_keybind_array[y]))
					{
						change_failed = true;
					}
				}
				//If the change did not fail, set the new keybind to the key pressed
				if (!change_failed)
				{
					changed_key = sf::Keyboard::delocalize(key_pressed_storage);
				}
				break;
			}

		}
	}
	key_pressed = true;
	last_changable_key_pressed = key_pressed_storage;
	last_fixed_key_pressed = sf::Keyboard::Key::Unknown;
	//Re-draw the keybind menu to reflect the new keybinds
	menu_vector[3].clear_menu_text();
	menu_vector[3].set_menu_text({ "Move Down/Left = " + sf::Keyboard::getDescription(fixed_keybind_array[1]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[1]).toAnsiString(), "Move Down = " + sf::Keyboard::getDescription(fixed_keybind_array[2]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[2]).toAnsiString(), "Move Down/Right = " + sf::Keyboard::getDescription(fixed_keybind_array[3]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[3]).toAnsiString(), "Move Left = " + sf::Keyboard::getDescription(fixed_keybind_array[4]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[4]).toAnsiString(), "Stand Still = " + sf::Keyboard::getDescription(fixed_keybind_array[5]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[5]).toAnsiString(), "Move Right = " + sf::Keyboard::getDescription(fixed_keybind_array[6]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[6]).toAnsiString(), "Move Up/Left = " + sf::Keyboard::getDescription(fixed_keybind_array[7]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[7]).toAnsiString(), "Move Up = " + sf::Keyboard::getDescription(fixed_keybind_array[8]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[8]).toAnsiString(), "Move Up/Right = " + sf::Keyboard::getDescription(fixed_keybind_array[9]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[9]).toAnsiString(), "Select = " + sf::Keyboard::getDescription(fixed_keybind_array[0]).toAnsiString() + "/" + sf::Keyboard::getDescription(changable_keybind_array[0]).toAnsiString(), "DEFAULT",  "Back" });
	menu_vector[3].set_scale(render_scale);
	menu_vector[3].set_outline_color(sf::Color::Magenta);
	menu_vector[3].set_position({ static_cast<float>(window.getSize().x) / 2, static_cast<float>(window.getSize().y) / 2 });

}