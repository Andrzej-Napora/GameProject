#include "Vortex.h"

#include <cmath>

namespace {
constexpr char VORTEX_TEXTURE_PATH[] = "resources/textures/vortex.png";
}

Vortex::Vortex(int randomValue)
    : force(20000.f), vortex(90.f), velocity(-200.f, 0.f), spawnTimer(0.f),
      xPosition(850.f), yPosition(600.f + randomValue) {
  texture.loadFromFile(VORTEX_TEXTURE_PATH);
  vortex.setTexture(&texture);
  vortex.setPosition({xPosition, yPosition});
}

sf::CircleShape Vortex::getShape() const { return vortex; }

void Vortex::update(float dt, int randomValue) {
  spawnTimer += dt;
  vortex.move(velocity.x * dt, velocity.y * dt);
  xPosition += velocity.x * dt;

  if (spawnTimer > 4.f) {
    spawnTimer = 0.f;
    if (vortex.getPosition().x < -50.f) {
      reset(randomValue);
    }
  }
}

sf::Vector2f Vortex::calculateGravity(const Bird &bird) const {
  sf::Vector2f gravityVector;
  const float yDistance = vortex.getPosition().y - bird.getYPosition();
  const float xDistance = vortex.getPosition().x - bird.getXPosition();
  float distance = std::sqrt(std::pow(xDistance, 2) + std::pow(yDistance, 2));

  if (distance < 80.f) {
    distance = 80.f;
  }

  const float currentForce = force / distance;
  gravityVector.x = currentForce * xDistance / distance;
  gravityVector.y = currentForce * yDistance / distance;
  return gravityVector;
}

void Vortex::reset(int randomValue) {
  yPosition =
      randomValue > 50 ? 400.f + 2.f * randomValue : 100.f + 3.f * randomValue;
  xPosition = 1000.f;
  spawnTimer = 0.f;
  vortex.setPosition({xPosition, yPosition});
}
