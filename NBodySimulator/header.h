#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <deque>
#include <queue>
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::deque;
using std::unique_ptr;
using std::pair;
using std::fstream;
using std::stringstream;

//rozne GameState'y, w ktorych bedzie znajdowac sie gra
enum class GameState {Starting_menu, Playing, Pause_menu, Ranking_list, Settings, Load_list, Game_Over, Username_Input
};

const int window_size = 800;

//Klasa zarzadzajaca oknem gry
class GameWindow
{
private:
	sf::RenderWindow window;
	unsigned int width;
	unsigned int height;
	sf::Texture tex;
	sf::Sprite sprite;
public:
	sf::Sprite getSprite();
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

//klasa obslugujaca nickname aktualnego uzytkownika
class User
{
private:
	string nickname;
	sf::Font font1;
	sf::Text nameText;
public:
	User(sf::Font& font);
	string& getName();
	sf::Text& getNameText();
	void setNickname(string& name);
	void updateNameText();
	void nameReset();
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
	Menu(string menu_title, int optionsSize, vector<pair<int,string>> labels, sf::Font& font);
	Menu(string menu_title, sf::Font& font);
	//gettery i settery
	const char& getMenuCount() const;
	const sf::Font& getFont() const;
	const sf::RectangleShape& getSelector() const;
	sf::RectangleShape& getSelectorMod();
	const sf::Text& getTitle() const;
	const vector<sf::Text>& getOptions() const;
	const vector<float>& getSelectorPositions() const;
	void optionsUpdate(vector <pair<int,string>> list);
};





//klasa zarzadzajaca ptakiem
class Bird
{
private:
	sf::Texture tex;
	sf::CircleShape birdShape;
	sf::Vector2f velocity;
	float xPosition;
	float yPosition;
	float radius;
	float gravity;
	float jump;
	float move;
	float distans;
public:
	Bird();
	float& getDistans();
	const float getXPosition() const;
	const float getYPosition() const;
	void setVelocity(float x, float y);
	const float getRadius() const;
	sf::CircleShape getBirdShape();
	void birdUpdate(float dt, sf::Vector2f vec);
	void jumpHandle();
	void leftMovementHandle();
	void rightMovementHandle();
	void resetBird();
};

class Score
{
protected:
	unsigned int score;
	sf::Text scoreText;
	sf::Font font;
public:
	Score();
	const int getScore() const;
	const sf::Text& getText() const;
	void setScore(int score);
	void incrementScore(float change);
};



class RankingList
{
private:
	const short listSize = 100;
	const string path = "../resources/RankingList/RankingList.txt";
	vector<pair<int, string>> list;

public:
	RankingList();
	vector<pair<int, string>>& getList();
	void rankingListUpdate(Score& score, User& user);
	void rankingListSave();
};

class Vortex
{
private:
	float force;
	sf::CircleShape vortex;
	sf::Texture tex;
	sf::Vector2f velocity;
	float spawnTimer;
	float xPosition;
	float yPosition;
public:
	Vortex(int randomValue);
	sf::CircleShape getVortex();
	void vortexUpdate(float dt,int randomValue);
	sf::Vector2f gravity(Bird bird);
};