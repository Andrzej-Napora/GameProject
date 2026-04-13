#include "header.h"

//konstruktor domyslny
GameWindow::GameWindow():width(800), height(800)
{
	window.create(sf::VideoMode( 800,800 ), "Flappy birds - Menu");
    window.setFramerateLimit(60);
}

//konstruktor parametryczny
void GameWindow::setWindow(unsigned int width_arg, unsigned int height_arg)
{
    width = width_arg;
    height = height_arg;

    window.create(sf::VideoMode( width,height ), "Flappy birds - Menu");
    window.setFramerateLimit(60);
}

//gettery settery
const unsigned int& GameWindow::getWidth() const { return width; }
const unsigned int& GameWindow::getHight() const { return height; }
void GameWindow::setWidth(unsigned int width) { this->width = width; }
void GameWindow::setHight(unsigned int height) { this->height = height; }
sf::RenderWindow& GameWindow::getWindow() { return window; }





Menu::Menu(string menu_title, int optionsSize, vector<string> labels, sf::Font& font_arg)
{
    float const startingPosition = 250.f;   //pozycja w ktorej startuje selector i pierwsza opcja wyboru

    font = font_arg;

    //ustawienia selectora
    selector.setSize({ 500.f,60.f });
    selector.setFillColor(sf::Color(255, 255, 255, 100));
    selector.setOrigin(250.f, 30.f);

    //ustawienia tytulu menu
    title.setFont(font);
    title.setString(menu_title);
    title.setCharacterSize(50);
    sf::FloatRect titleFrame = title.getLocalBounds();
    title.setOrigin(titleFrame.left + titleFrame.width / 2.f, titleFrame.top + titleFrame.height / 2.f);
    title.setPosition({ 400.f,50.f });

    //petla dodajaca do vectora options rozne skladowe menu, np. load_game, ranking_list itd.,
    //dodatkowo uzupelniany jest tu vector selectorPositions
    for (int i = 0; i <= optionsSize;i++)
    {
        sf::Text text(labels[i], font, 50);
        sf::FloatRect Frame = text.getLocalBounds();
        text.setOrigin(Frame.left + Frame.width / 2.f, Frame.top + Frame.height / 2.f);
        text.setPosition({ 400.f,(startingPosition+(100*i))});
        options.push_back(text);
        selectorPositions.push_back(startingPosition + (100 * i));
    }

    menuCount = options.size();
}
//gettery
const char& Menu::getMenuCount() const { return menuCount; }
const sf::Font& Menu::getFont() const { return font; }
const sf::RectangleShape& Menu::getSelector() const { return selector; }
sf::RectangleShape& Menu::getSelectorMod() { return selector; }
const sf::Text& Menu::getTitle() const { return title; }
const vector<sf::Text>& Menu::getOptions() const { return options; }
const vector<float>& Menu::getSelectorPositions() const { return selectorPositions; }




//konstruktor prostego ptaka
Bird::Bird() : birdShape({ 40.f,40.f }), velocity(0.f,0.f), gravity(30.f),jump(-2000.f)
{
    birdShape.setFillColor(sf::Color::Yellow);
    birdShape.setOutlineColor(sf::Color::White);
    birdShape.setOrigin({20.f,20.f});
    birdShape.setPosition({ 200.f,400.f });
};
sf::RectangleShape Bird::getBirdShape() { return birdShape; }

void Bird::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}
void Bird::birdUpdate(float dt)
{
    //zmienna dt zapewnia, ze ruch obiektu bedzie zalezny od czasu, ktory minal od ostatniego update,
    //a nie od tego jak szybko dziala komputer   
    birdShape.move(velocity.x * dt, (velocity.y + gravity) *dt);
    velocity = { 0.f,0.f };
}
void Bird::inputHandle()
{
    velocity.y += jump;
}





//konstruktor prostych przeszkod
Obstacle::Obstacle(float xPosition, float yPosition, float xSize, float ySize, float velX, float velY):
    xPosition(xPosition), yPosition(yPosition), xSize(xSize), ySize(ySize)
{
    velocity = { velX,velY };
    obstacle.setSize({ xSize,ySize });
    obstacle.setPosition({ xPosition, yPosition });
    obstacle.setFillColor(sf::Color::White);
}

const sf::RectangleShape& Obstacle::getObs() const { return obstacle; }

void Obstacle::updateObstacle(float dt)
{
    //zmienna dt zapewnia, ze ruch obiektu bedzie zalezny od czasu, ktory minal od ostatniego update,
    //a nie od tego jak szybko dziala komputer   
    obstacle.move( velocity.x * dt,velocity.y * dt);
}






ObstacleQueue::ObstacleQueue() :spawnTimer(0.f){}

float& ObstacleQueue::getSpawnTimer() { return spawnTimer; }

deque<Obstacle>& ObstacleQueue::getQueue(){return obstacleQueue;}

//prymitywny generator losowych przeszkod
void ObstacleQueue::addRandomObstacle(int randomValue)
{
    sf::Vector2f velocity = {-50.f,0.f};
    float yPosition = 8 * randomValue;
    float xPosition = 800;
    float xSize = 50;
    float ySize = 200 + 2 * randomValue;
    Obstacle obs(xPosition, yPosition, xSize, ySize, velocity.x, velocity.y);
    obstacleQueue.push_back(obs);
}
