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

private:
  std::pair<Obstacle, Obstacle> obstacles;
};

class ObstacleQueue {
public:
  ObstacleQueue();

  [[nodiscard]] std::deque<DoubleObstacle> &getQueue();

  void removeOutOfScreenObstacles(float dt);
  void spawnObstacleIfNeeded(float dt, int randomValue);
  void reset();

private:
  void addRandomObstacle(int randomValue);

  std::deque<DoubleObstacle> obstacleQueue;
  float spawnTimer = 0.f;
  float removeTimer = 0.f;
  sf::Texture pipeTexture;
};
