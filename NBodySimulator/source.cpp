#include "header.h"


//********************************************************************************************************************
//Klasa GameWindow
//********************************************************************************************************************

//konstruktor domyslny
GameWindow::GameWindow():width(window_size), height(window_size)
{
	window.create(sf::VideoMode(window_size, window_size), "Flappy birds - Menu");
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

//********************************************************************************************************************
//Klasa Menu
//********************************************************************************************************************

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




//********************************************************************************************************************
//Klasa Bird
//********************************************************************************************************************

//konstruktor prostego ptaka
Bird::Bird() : birdShape(45.f), velocity(0.f,0.f), gravity(550.f),jump(-250.f)
{
    radius = 45.f;
    birdShape.setFillColor(sf::Color::Yellow);
    birdShape.setOutlineColor(sf::Color::White);
    birdShape.setOrigin({20.f,20.f});
    birdShape.setPosition({ 200.f,400.f });
};
sf::CircleShape Bird::getBirdShape() { return birdShape; }
const float Bird::getRadius() const { return radius; }

void Bird::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}
void Bird::birdUpdate(float dt)
{
    velocity.y += gravity * dt;
    birdShape.move(velocity.x * dt, velocity.y * dt);
}

void Bird::inputHandle()
{
    velocity.y = jump;
}


//********************************************************************************************************************
//Klasa Obstacle
//********************************************************************************************************************

//konstruktor prostych przeszkod
Obstacle::Obstacle(float xPosition, float yPosition, float xSize, float ySize, float velX, float velY):
    xPosition(xPosition), yPosition(yPosition), xSize(xSize), ySize(ySize)
{
    velocity = { velX,velY };
    obstacle.setSize({ xSize,ySize });
    obstacle.setPosition({ xPosition, yPosition });
    obstacle.setFillColor(sf::Color::White);
}

const float Obstacle::getXSize() const { return xSize; }
const float Obstacle::getYSize() const { return ySize; }
const float Obstacle::getXPosition() const { return xPosition; }
const float Obstacle::getYPosition() const { return yPosition; }
const sf::RectangleShape& Obstacle::getObstacle() const { return obstacle; }

void Obstacle::updateObstacle(float dt)
{
    //zmienna dt zapewnia, ze ruch obiektu bedzie zalezny od czasu, ktory minal od ostatniego update,
    //a nie od tego jak szybko dziala komputer   
    obstacle.move( velocity.x * dt,velocity.y * dt);
    xPosition += velocity.x * dt;
    yPosition += velocity.y * dt;
}

//********************************************************************************************************************
//Klasa DoubleObstacle
//********************************************************************************************************************

vector<Obstacle>& DoubleObstacle::getdoubleObs() { return doubleObs; }

//funkcja sprawdzajaca, czy ptak dotknal ktoras z przeszkod
bool DoubleObstacle::collisionCheck(Bird& bird)
{
    float upperYBird = bird.getBirdShape().getPosition().y - bird.getRadius()+30.f;
    float bottomYBird = bird.getBirdShape().getPosition().y + bird.getRadius()-30.f;
    float upperYObs = doubleObs[1].getYPosition();
    float bottomYObs = doubleObs[1].getYPosition() + doubleObs[1].getYSize();

    float upperYObs2 = doubleObs[0].getYPosition();
    float bottomYObs2 = doubleObs[0].getYPosition() + doubleObs[0].getYSize();

    std::cout << " collisionCheck "<<endl;
    std::cout << "Ptak:          top=" << upperYBird << " bottom=" << bottomYBird << endl;
    std::cout << "Obs[1] gorna:  top=" << upperYObs << " bottom=" << bottomYObs << endl;
    std::cout << "Obs[0] dolna:  top=" << upperYObs2 << " bottom=" << bottomYObs2 << endl;

    bool kolizjaGorna = (upperYObs < upperYBird && upperYBird < bottomYObs)
        || (upperYObs < bottomYBird && bottomYBird < bottomYObs);

    bool kolizjaDolna = (upperYObs2 < upperYBird && upperYBird < bottomYObs2)
        || (upperYObs2 < bottomYBird && bottomYBird < bottomYObs2);

    std::cout << "kolizjaGorna=" << kolizjaGorna << " kolizjaDolna=" << kolizjaDolna << endl;
    if (kolizjaGorna || kolizjaDolna)
        return true;
    else
        return false;

}


//********************************************************************************************************************
//Klasa ObstacleQueue
//********************************************************************************************************************

ObstacleQueue::ObstacleQueue() :spawnTimer(0.f){}

float& ObstacleQueue::getSpawnTimer() { return spawnTimer; }

deque<DoubleObstacle>& ObstacleQueue::getQueue(){return obstacleQueue;}

float& ObstacleQueue::getRemoveTimer() { return removeTimer; }


//co dwie sekund usuwamy wszystkie przeszkoda, ktore wyszly poza lewa krawedz ekranu
void ObstacleQueue::removeObstacleCondition(float dt)
{
    removeTimer += dt;
    if (removeTimer > 2)
    {
        while (obstacleQueue[0].getdoubleObs()[0].getXPosition() < -50)
            obstacleQueue.pop_front();
        removeTimer = 0;
    }
}

//co dwie sekundy generujemy nowa przeszkode
void ObstacleQueue::spawnObstacleCondition(float dt, int randomValue)
{
    spawnTimer += dt;
    if (spawnTimer > 2)
    {
        addRandomObstacle(randomValue);
        spawnTimer = 0;
    }
}

//generator losowych przeszkod
void ObstacleQueue::addRandomObstacle(int randomValue)
{
    DoubleObstacle doubleObs;

    sf::Vector2f velocity = {-100.f,0.f};
    float xPosition = 800;

    //przeszkoda dolna
    float yPosition = 600-(1.5*randomValue);
    float xSize = 50;
    float ySize =window_size - yPosition;
    Obstacle obsBottom(xPosition, yPosition, xSize, ySize, velocity.x, velocity.y);
    doubleObs.getdoubleObs().push_back(obsBottom);

    //przeszkoda gorna
    yPosition = 0;
    xSize = 50;
    ySize = 300 - (1.5 * randomValue);
    Obstacle obsUpper(xPosition, yPosition, xSize, ySize, velocity.x, velocity.y);
    doubleObs.getdoubleObs().push_back(obsUpper);

    obstacleQueue.push_back(std::move(doubleObs));

}
//funkcja sprawdzajaca pomiedzy, ktorymi dwiema przeszkodami aktualnie znajduje sie ptak
DoubleObstacle* ObstacleQueue::birdBetweenObstacles(Bird& bird)
{
    for (auto& doubleObs : obstacleQueue)
    {
        float leftXBird = bird.getBirdShape().getPosition().x - bird.getRadius()+30.f;
        float rightXBird = bird.getBirdShape().getPosition().x + bird.getRadius()-30.f;
        float leftXObs = doubleObs.getdoubleObs()[0].getXPosition();
        float rightXObs = doubleObs.getdoubleObs()[0].getXPosition() + doubleObs.getdoubleObs()[0].getXSize();
        if (leftXObs < leftXBird && leftXBird < rightXObs || leftXObs < rightXBird && rightXBird < rightXObs)
            return &doubleObs;
    }
    return nullptr;
}
//********************************************************************************************************************
//Klasa Score
//********************************************************************************************************************

Score::Score() :score(0)
{
    scoreText.setString(std::to_string(score));
    scoreText.setCharacterSize(30);
    font.loadFromFile("Cabin-SemiBold.ttf");
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::Cyan);
    scoreText.setPosition({ 700.f,50.f });
}

const unsigned int Score::getScore() const { return score; }
const sf::Text& Score::getText() const { return scoreText; }
void Score::setScore(unsigned int score) { this->score = score; }
void Score::incrementScore(float change) 
{ 
    score += 60 * change;
    scoreText.setString(std::to_string(score));
}