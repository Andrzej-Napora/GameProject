#include "Bird.h"

namespace {
constexpr char BIRD_TEXTURE_PATH[] = "resources/textures/bird.png";
const sf::Vector2f START_POSITION = {200.f, 400.f};
} // namespace

Bird::Bird()
    : velocity(0.f, 0.f), xPosition(START_POSITION.x),
      yPosition(START_POSITION.y), gravity(550.f), jumpVelocity(-330.f),
      horizontalMove(100.f) {
  texture.loadFromFile(BIRD_TEXTURE_PATH);
  birdShape.setTexture(&texture);
  radius = static_cast<float>(texture.getSize().x) / 2.0f;
  birdShape.setRadius(45.f);
  birdShape.setFillColor(sf::Color::Yellow);
  birdShape.setOutlineColor(sf::Color::White);
  birdShape.setOrigin({20.f, 20.f});
  birdShape.setPosition(START_POSITION);
}

sf::CircleShape Bird::getBirdShape() const { return birdShape; }
float Bird::getRadius() const { return radius; }
float Bird::getXPosition() const { return xPosition; }
float Bird::getYPosition() const { return yPosition; }

void Bird::setVelocity(float x, float y) {
  velocity.x = x;
  velocity.y = y;
}

void Bird::update(float dt, sf::Vector2f externalForce) {
  constexpr float brakes = 70.f;

  velocity.y += gravity * dt;
  birdShape.move((velocity.x + externalForce.x) * dt,
                 (velocity.y + externalForce.y) * dt);
  xPosition += (velocity.x + externalForce.x) * dt;
  yPosition += (velocity.y + externalForce.y) * dt;

  if (velocity.x > 0.f) {
    velocity.x -= brakes * dt;
  } else if (velocity.x < 0.f) {
    velocity.x += brakes * dt;
  }
}

void Bird::jumpHandle() { velocity.y = jumpVelocity; }
void Bird::leftMovementHandle() { velocity.x = -horizontalMove; }
void Bird::rightMovementHandle() { velocity.x = horizontalMove; }

void Bird::reset() {
  birdShape.setPosition(START_POSITION);
  xPosition = START_POSITION.x;
  yPosition = START_POSITION.y;
  velocity = {0.f, 0.f};
}
