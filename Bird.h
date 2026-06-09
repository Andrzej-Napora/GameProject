#pragma once

#include <SFML/Graphics.hpp>

class Bird {
public:
  Bird();

  [[nodiscard]] float getXPosition() const;
  [[nodiscard]] float getYPosition() const;
  [[nodiscard]] float getRadius() const;
  [[nodiscard]] sf::CircleShape getBirdShape() const;

  void setVelocity(float x, float y);
  void update(float dt, sf::Vector2f externalForce);
  void jumpHandle();
  void leftMovementHandle();
  void rightMovementHandle();
  void reset();

private:
  sf::Texture texture;
  sf::CircleShape birdShape;
  sf::Vector2f velocity;
  float xPosition;
  float yPosition;
  float radius = 0.f;
  float gravity;
  float jumpVelocity;
  float horizontalMove;
};
