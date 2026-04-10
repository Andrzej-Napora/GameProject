#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

enum class GameState {Starting_menu, Playing, Pause_menu, Ranking_list, Settings, Load_list};


class GameWindow
{
private:
	sf::RenderWindow window;
	unsigned int width;
	unsigned int hight;
public:
	const unsigned int& getWidth() const;
	const unsigned int& getHight() const;
	void setWidth(unsigned int width);
	void setHight(unsigned int hight);
	GameWindow();
	void setWindow(unsigned int width, unsigned int hight);
	sf::RenderWindow& getWindow();
};

class Menu
{
private:
	char menuCount;
	sf::Font font;
	sf::RectangleShape selector;
	sf::Text title;
	vector<sf::Text> options;
	vector<float> selectorPositions;
public:
	Menu(string menu_title, int optionsSize, vector<string> labels, sf::Font& font);
	const char& getMenuCount() const;
	const sf::Font& getFont() const;
	const sf::RectangleShape& getSelector() const;
	sf::RectangleShape& getSelectorMod();
	const sf::Text& getTitle() const;
	const vector<sf::Text>& getOptions() const;
	const vector<float>& getSelectorPositions() const;
};
