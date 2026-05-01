#pragma once

#include "header.h"
#include "Obstacles.h"

//pojedyncza przeszkoda
class Obstacle
{
private:
	sf::RectangleShape obstacle;
	sf::Vector2f velocity;
	float yPosition;
	float xPosition;
	float xSize;
	float ySize;
public:
	Obstacle() = default;
	Obstacle(float xPosition, float yPosition, float xSize, float ySize, float velX, float velY);
	const float getXPosition() const;
	const float getYPosition() const;
	const float getXSize() const;
	const float getYSize() const;
	sf::RectangleShape& getObstacle();
	void updateObstacle(float dt);
};

//podwojna przeszkoda
class DoubleObstacle
{
private:
	pair<Obstacle, Obstacle> doubleObs;
public:
	DoubleObstacle() = default;
	pair<Obstacle, Obstacle>& getdoubleObs();
	//bool collisionCheck(Bird& bird);
};



//zestaw przeszkod
class ObstacleQueue
{
private:
	deque<DoubleObstacle> obstacleQueue;
	float spawnTimer;
	float removeTimer;
	sf::Texture pipe;
public:
	ObstacleQueue();
	float& getSpawnTimer();
	float& getRemoveTimer();
	deque<DoubleObstacle>& getQueue();
	void addRandomObstacle(int randomValue);
	void obstacleRemover();
	void removeObstacleCondition(float dt);
	void spawnObstacleCondition(float dt, int randomValue);
	DoubleObstacle* birdBetweenObstacles(Bird& bird);
	void resetObstacleQueue();
};