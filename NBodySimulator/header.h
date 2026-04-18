#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <deque>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::deque;

//rozne GameState'y, w ktorych bedzie znajdowac sie gra
enum class GameState {Starting_menu, Playing, Pause_menu, Ranking_list, Settings, Load_list};

const int window_size = 800;





//Klasa zarzadzajaca oknem gry
class GameWindow
{
private:
	sf::RenderWindow window;
	unsigned int width;
	unsigned int height;
public:
	const unsigned int& getWidth() const;
	const unsigned int& getHight() const;
	void setWidth(unsigned int width);
	void setHight(unsigned int height);
	sf::RenderWindow& getWindow();
	//konstruktor domyslny tworzy okno o wymiarach 800x800
	GameWindow();
	//funkcja tworzaca okno o dowolnych wymiarach
	void setWindow(unsigned int width, unsigned int height);
};






//klasa do tworzenia roznych menu, np menu startowego i menu pause
class Menu
{
private:
	char menuCount;		//ilosc opcji wyboru w danym menu
	sf::Font font;
	sf::RectangleShape selector;	//prostokat wyboru reagujacy na klawisze W/S
	sf::Text title;
	vector<sf::Text> options;	//vector przechowujacy informacje o poszczegolnych skladowych menu, np ranking_list,settings itd.
	vector<float> selectorPositions;	//vector przechowujacy pozycje, na ktore ma przemieszczac sie selector
public:
	//glowny konstruktor menu
	Menu(string menu_title, int optionsSize, vector<string> labels, sf::Font& font);
	//gettery i settery
	const char& getMenuCount() const;
	const sf::Font& getFont() const;
	const sf::RectangleShape& getSelector() const;
	sf::RectangleShape& getSelectorMod();
	const sf::Text& getTitle() const;
	const vector<sf::Text>& getOptions() const;
	const vector<float>& getSelectorPositions() const;
};





//klasa zarzadzajaca ptakiem
class Bird
{
private:
	sf::RectangleShape birdShape;
	sf::Vector2f velocity;
	float gravity;
	float jump;
public:
	Bird();
	void setVelocity(float x, float y);
	sf::RectangleShape getBirdShape();
	void birdUpdate(float dt);
	void inputHandle();
};





//pojedyncza przeszkoda
class Obstacle
{
protected:
	sf::RectangleShape obstacle;
	sf::Vector2f velocity;
	float yPosition;
	float xPosition;
	float xSize;
	float ySize;
public:
	Obstacle(float xPosition, float yPosition, float xSize, float ySize, float velX, float velY);
	float getXPosition();
	const sf::RectangleShape& getObstacle() const;
	void updateObstacle(float dt);
};





//zestaw przeszkod
class ObstacleQueue
{
private:
	deque<Obstacle> obstacleQueue;
	float spawnTimer;
	float removeTimer;
public:
	ObstacleQueue();
	float& getSpawnTimer();
	float& getRemoveTimer();
	deque<Obstacle>& getQueue();
	void addRandomObstacle(int randomValue);
	void obstacleRemover();
	void removeObstacleCondition(float dt);
	void spawnObstacleCondition(float dt, int randomValue);
};


