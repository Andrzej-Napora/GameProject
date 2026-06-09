#pragma once

#include "Bird.h"

#include <SFML/Graphics.hpp>

class Vortex {
public:
  explicit Vortex(int randomValue);

  [[nodiscard]] sf::CircleShape getShape() const;
  [[nodiscard]] sf::Vector2f calculateGravity(const Bird &bird) const;

  void update(float dt, int randomValue);
  void reset(int randomValue);

private:
  float force;
  sf::CircleShape vortex;
  sf::Texture texture;
  sf::Vector2f velocity;
  float spawnTimer;
  float xPosition;
  float yPosition;
};
