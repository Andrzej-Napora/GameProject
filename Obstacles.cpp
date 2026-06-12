#include "Obstacles.h"

#include "GameConstants.h"
#include <cmath>

namespace {
constexpr char PIPE_TEXTURE_PATH[] = "resources/textures/pipe.png";
constexpr float OBSTACLE_WIDTH = 50.f;
constexpr float SCREEN_RIGHT_EDGE = 800.f;
}

Obstacle::Obstacle(float newXPosition, float newYPosition, float newXSize,
                   float newYSize, float velocityX, float velocityY)
    : velocity(velocityX, velocityY), yPosition(newYPosition),
      xPosition(newXPosition), xSize(newXSize), ySize(newYSize) {
  shape.setSize({xSize, ySize});
  shape.setPosition({xPosition, yPosition});
  shape.setFillColor(sf::Color::White);
}

float Obstacle::getXSize() const { return xSize; }
float Obstacle::getYSize() const { return ySize; }
float Obstacle::getXPosition() const { return xPosition; }
float Obstacle::getYPosition() const { return yPosition; }
sf::RectangleShape &Obstacle::getShape() { return shape; }

void Obstacle::update(float dt) {
  shape.move(velocity.x * dt, velocity.y * dt);
  xPosition += velocity.x * dt;
  yPosition += velocity.y * dt;
}

void Obstacle::setVelocity(float velocityX, float velocityY) {
  velocity.x = velocityX;
  velocity.y = velocityY;
}

std::pair<Obstacle, Obstacle> &DoubleObstacle::getObstacles() {
  return obstacles;
}

bool DoubleObstacle::isPassed() const {
  return passed;
}

void DoubleObstacle::setPassed(bool state) {
  passed = state;
}

ObstacleQueue::ObstacleQueue() : spawnTimer(2.f) {
  pipeTexture.loadFromFile(PIPE_TEXTURE_PATH);
}

std::deque<DoubleObstacle> &ObstacleQueue::getQueue() { return obstacleQueue; }

float ObstacleQueue::getCurrentSpeed() const { return currentSpeed; }

void ObstacleQueue::reset() {
  spawnTimer = 0.f;
  removeTimer = 0.f;
  currentSpeed = -150.f;
  obstacleQueue.clear();
}

void ObstacleQueue::updateSpeed(int currentScore, bool isSlowed) {
  const float baseSpeed = -150.f - (static_cast<float>(currentScore) * 6.f);
  currentSpeed = isSlowed ? baseSpeed * 0.4f : baseSpeed;

  for (auto &doubleObstacle : obstacleQueue) {
    doubleObstacle.getObstacles().first.setVelocity(currentSpeed, 0.f);
    doubleObstacle.getObstacles().second.setVelocity(currentSpeed, 0.f);
  }
}

void ObstacleQueue::removeOutOfScreenObstacles(float dt) {
  removeTimer += dt;
  if (removeTimer <= 2.f) {
    return;
  }

  while (!obstacleQueue.empty() &&
         obstacleQueue.front().getObstacles().first.getXPosition() <
             -OBSTACLE_WIDTH) {
    obstacleQueue.pop_front();
  }
  removeTimer = 0.f;
}

void ObstacleQueue::spawnObstacleIfNeeded(float dt, int randomValue) {
  spawnTimer += dt;
  const float targetInterval = 350.f / std::abs(currentSpeed);
  if (spawnTimer > targetInterval) {
    addRandomObstacle(randomValue);
    spawnTimer = 0.f;
  }
}

void ObstacleQueue::addRandomObstacle(int randomValue) {
  DoubleObstacle doubleObstacle;
  const sf::Vector2f velocity = {currentSpeed, 0.f};

  const float upperHeight = 50.f + (4.0f * randomValue);
  const float bottomYPosition = upperHeight + 250.f;
  const float bottomHeight = WINDOW_SIZE - bottomYPosition;

  Obstacle bottomObstacle(SCREEN_RIGHT_EDGE, bottomYPosition, OBSTACLE_WIDTH,
                          bottomHeight, velocity.x, velocity.y);
  bottomObstacle.getShape().setTexture(&pipeTexture);
  doubleObstacle.getObstacles().second = bottomObstacle;

  Obstacle upperObstacle(SCREEN_RIGHT_EDGE, 0.f, OBSTACLE_WIDTH, upperHeight,
                         velocity.x, velocity.y);
  upperObstacle.getShape().setTexture(&pipeTexture);
  upperObstacle.getShape().setOrigin(0.f, upperHeight);
  upperObstacle.getShape().setScale(1.f, -1.f);
  doubleObstacle.getObstacles().first = upperObstacle;

  obstacleQueue.push_back(doubleObstacle);
}