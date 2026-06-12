#pragma once

#include "Bird.h"

#include <SFML/Graphics.hpp>
#include <deque>
#include <utility>

class Obstacle {
public:
  Obstacle() = default;
  Obstacle(float xPosition, float yPosition, float xSize, float ySize,
           float velocityX, float velocityY);

  [[nodiscard]] float getXPosition() const;
  [[nodiscard]] float getYPosition() const;
  [[nodiscard]] float getXSize() const;
  [[nodiscard]] float getYSize() const;
  [[nodiscard]] sf::RectangleShape &getShape();

  void update(float dt);
  void setVelocity(float velocityX, float velocityY);

private:
  sf::RectangleShape shape;
  sf::Vector2f velocity;
  float yPosition = 0.f;
  float xPosition = 0.f;
  float xSize = 0.f;
  float ySize = 0.f;
};

class DoubleObstacle {
public:
  [[nodiscard]] std::pair<Obstacle, Obstacle> &getObstacles();
  [[nodiscard]] bool isPassed() const;

  void setPassed(bool state);

private:
  std::pair<Obstacle, Obstacle> obstacles;
  bool passed = false;
};

class ObstacleQueue {
public:
  ObstacleQueue();

  [[nodiscard]] std::deque<DoubleObstacle> &getQueue();
  [[nodiscard]] float getCurrentSpeed() const;

  void removeOutOfScreenObstacles(float dt);
  void spawnObstacleIfNeeded(float dt, int randomValue);
  void reset();
  void updateSpeed(int currentScore, bool isSlowed = false);

private:
  void addRandomObstacle(int randomValue);

  std::deque<DoubleObstacle> obstacleQueue;
  float spawnTimer = 0.f;
  float removeTimer = 0.f;
  sf::Texture pipeTexture;
  float currentSpeed = -150.f;
};