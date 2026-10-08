#include "Menu.hpp"



Menu::Menu()
{
	menu_index = 1;
	menu_selection = 0;
	background.setSize({ 1, 1 });
	background.setFillColor(sf::Color(0, 0, 0, 255));
	background.setScale({ 1, 1 });

}

void Menu::set_menu_index(int new_index)
{
	menu_index = new_index;
}
void Menu::set_position(sf::Vector2f new_position)
{
	sf::Text longest_string(font);
	longest_string.setString("");
	//sf::Vector2f offset = background.getPosition() - new_position;
	longest_string.setCharacterSize(menu_text[0].getCharacterSize());
	menu_text[menu_selection].setOutlineThickness(0);
	for (int x = 0; x < static_cast<int>(menu_text.size()); x++)
	{
		//menu_text[x].setCharacterSize(90);

		if (menu_text[x].getString().getSize() > longest_string.getString().getSize())
		{
			longest_string.setString(menu_text[x].getString());
		}
		//menu_text[x].setPosition(menu_text[x].getPosition() + offset);
		//std::cout << "DEBUG: Setting menu_text[" << x << "] position to: " << menu_text[x].getPosition().x << ", " << menu_text[x].getPosition().y << "'\n";
	}
	background.setSize({ static_cast<float>(longest_string.getLocalBounds().size.x), static_cast<float>(longest_string.getLocalBounds().size.y * 2.0f * menu_text.size()) });
	background.setPosition({ new_position.x - background.getLocalBounds().size.x / 2, new_position.y - background.getLocalBounds().size.y / 2 });

	for (int x = 0; x < static_cast<int>(menu_text.size()); x++)
	{
		menu_text[x].setPosition({background.getPosition().x - (menu_text[x].getLocalBounds().size.x / 2) + background.getLocalBounds().size.x / 2, (background.getPosition().y + longest_string.getLocalBounds().size.y * 2.0f * x)});
	}
	menu_text[menu_selection].setOutlineThickness(10);

}
void Menu::set_background_size(sf::Vector2f new_size)
{
	background.setSize(new_size);
}
void Menu::set_fill_color(sf::Color new_color)
{
	background.setFillColor(new_color);
}
void Menu::set_scale(sf::Vector2f new_scale)
{
	//background.setScale({ new_scale.x / 10, new_scale.y / 10 });
	//for (int x = 0; x < static_cast<int>(menu_text.size()); x++)
	//{
	//	menu_text[x].setScale({ new_scale.x / 50, new_scale.y / 50 });
	//}
	for (int x = 0; x < static_cast<int>(menu_text.size()); x++)
	{
		menu_text[x].setCharacterSize(new_scale.x);
	}
}
void Menu::set_outline_color(sf::Color new_color)
{
	for (int x = 0; x < static_cast<int>(menu_text.size()); x++)
	{
		menu_text[x].setOutlineColor(new_color);
	}
}
void Menu::set_menu_text(std::vector<std::string> new_menu_text)
{
	//int longest_string_index = 0;
	//sf::Text longest_string(font);
	//longest_string.setString("");
	menu_selection = 0;
	for (int x = 0; x < static_cast<int>(new_menu_text.size()); x++)
	{
		menu_text.push_back(sf::Text(font));
		menu_text[x].setString(new_menu_text[x]);
		//if (new_menu_text[x].length() > longest_string.getString().getSize())
		//{
		//	longest_string.setString(new_menu_text[x]);
		//	longest_string_index = x;
		//}
		//menu_text[x].setPosition({ background.getPosition().x + (background.getGlobalBounds().size.x / 2) - (menu_text[x].getGlobalBounds().size.x / 2), background.getPosition().y + (menu_text[x].getGlobalBounds().size.y * ((2 * x))) });
	}
	//std::cout << "Setting background size to: " << longest_string.getGlobalBounds().size.x << ", " << longest_string.getGlobalBounds().size.y * static_cast<float>(menu_text.size()) << "'\n";
	//std::cout<< "longest string size: " << longest_string.getGlobalBounds().size.x << ", " << longest_string.getGlobalBounds().size.y << "'\n";
	//background.setSize({ longest_string.getGlobalBounds().size.x, longest_string.getGlobalBounds().size.y * static_cast<float>(menu_text.size())});
	//background.setSize({ 10, 10 });

}
void Menu::clear_menu_text() 
{
	menu_text[menu_selection].setOutlineThickness(0);
	menu_text.clear();
}

int Menu::get_menu_index()
{
	return menu_index;
}
sf::Vector2f Menu::get_size()
{
	return background.getGlobalBounds().size;
}
void Menu::move_up()
{
	menu_text[menu_selection].setOutlineThickness(0);
	menu_selection--;
	if (menu_selection < 0)
	{
		menu_selection = static_cast<int>(menu_text.size()) - 1;
	}
	menu_text[menu_selection].setOutlineThickness(10);
}
void Menu::move_down()
{
	menu_text[menu_selection].setOutlineThickness(0);
	menu_selection++;
	if (menu_selection > static_cast<int>(menu_text.size()) - 1)
	{
		menu_selection = 0;
	}
	menu_text[menu_selection].setOutlineThickness(10);
}
int Menu::select()
{
	switch (menu_index)
	{
		//Pause Menu
	case 1:
		switch (menu_selection)
		{
			//Resume
		case 0:
			return 0; break;
			//Options
		case 1:
			return 2; break;
			//Exit Game
		case 2:
			return -1; break;
		case 3:
			return -99; break;
			//Default to Resume
		default:
			return 0; break;
		}
		break;
		//Options Menu
	case 2:
		switch (menu_selection)
		{
			//Toggle Fullscreen
		case 0:
			return -2; break;
			//Keybinds
		case 1:
			return 3; break;
			//Return
		case 2:
			return 1; break;
			//Default to Return
		default:
			return 1; break;
		}
		break;
	case 3:
		switch (menu_selection)
		{
		case 0:
			return 91; break;
		case 1:
			return 92; break;
		case 2: 
			return 93; break;
		case 3:
			return 94; break;
		case 4:
			return 95; break;
		case 5:
			return 96; break;
		case 6:
			return 97; break;
		case 7:
			return 98; break;
		case 8:
			return 99; break;
		case 9:
			return 90; break;
		case 10:
			return -3; break;
			//Return
		case 11:
			return 2; break;
			//Default to Return
		default:
			return 2; break;
		}
	default:
		return -1;
		break;
	}
}
void Menu::menu_change()
{
	menu_text[menu_selection].setOutlineThickness(0);
	menu_selection = 0;
	menu_text[menu_selection].setOutlineThickness(10);
}

void Menu::draw(sf::RenderWindow& window)
{
	window.draw(background);
	for (int x = 0; x < static_cast<int>(menu_text.size()); x++)
	{
		window.draw(menu_text[x]);
	}
}
Menu::~Menu()
{

}
