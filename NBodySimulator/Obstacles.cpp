#include "header.h"
#include "Obstacles.h"

//********************************************************************************************************************
//Klasa Obstacle
//********************************************************************************************************************

//konstruktor prostych przeszkod
Obstacle::Obstacle(float xPosition, float yPosition, float xSize, float ySize, float velX, float velY) :
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
sf::RectangleShape& Obstacle::getObstacle() { return obstacle; }

void Obstacle::updateObstacle(float dt)
{
    //zmienna dt zapewnia, ze ruch obiektu bedzie zalezny od czasu, ktory minal od ostatniego update,
    //a nie od tego jak szybko dziala komputer   
    obstacle.move(velocity.x * dt, velocity.y * dt);
    xPosition += velocity.x * dt;
    yPosition += velocity.y * dt;
}

//********************************************************************************************************************
//Klasa DoubleObstacle
//********************************************************************************************************************

pair<Obstacle, Obstacle>& DoubleObstacle::getdoubleObs() { return doubleObs; }


//********************************************************************************************************************
//Klasa ObstacleQueue
//********************************************************************************************************************

ObstacleQueue::ObstacleQueue() :spawnTimer(2.f) {}

float& ObstacleQueue::getSpawnTimer() { return spawnTimer; }

deque<DoubleObstacle>& ObstacleQueue::getQueue() { return obstacleQueue; }

float& ObstacleQueue::getRemoveTimer() { return removeTimer; }

void ObstacleQueue::resetObstacleQueue()
{
    spawnTimer = 0;
    obstacleQueue.clear();
}

//co dwie sekund usuwamy wszystkie przeszkoda, ktore wyszly poza lewa krawedz ekranu
void ObstacleQueue::removeObstacleCondition(float dt)
{
    removeTimer += dt;
    if (removeTimer > 2)
    {
        while (!obstacleQueue.empty() && obstacleQueue.front().getdoubleObs().first.getXPosition() < -50)
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
    pipe.loadFromFile("../resources/textures/pipe.png");

    sf::Vector2f velocity = { -100.f,0.f };
    float xPosition = 800;

    //przeszkoda dolna
    float yPosition = 600 - (1.5 * randomValue);
    float xSize = 50;
    float ySize = window_size - yPosition;
    Obstacle obsBottom(xPosition, yPosition, xSize, ySize, velocity.x, velocity.y);
    obsBottom.getObstacle().setTexture(&pipe);
    doubleObs.getdoubleObs().second = obsBottom;

    //przeszkoda gorna
    yPosition = 0;
    xSize = 50;
    ySize = 300 - (1.5 * randomValue);
    Obstacle obsUpper(xPosition, yPosition, xSize, ySize, velocity.x, velocity.y);
    obsUpper.getObstacle().setTexture(&pipe);
    obsUpper.getObstacle().setOrigin(0.f, ySize);
    obsUpper.getObstacle().setScale(1.f, -1.f);
    doubleObs.getdoubleObs().first = obsUpper;

    obstacleQueue.push_back(doubleObs);

}