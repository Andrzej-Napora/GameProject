#include "PowerUp.h"

bool PowerUp::isActive() const { return this->active; }
sf::Sprite PowerUp::getSprite() const { return this->sprite; }
sf::FloatRect PowerUp::getBounds() const { return this->sprite.getGlobalBounds(); }

void PowerUp::update(float dt, float speed) {
    if(!this->active) return;
    this->sprite.move(speed * dt, 0.f);
    if(this->sprite.getPosition().x < -100.f) this->active = false;
}

void PowerUp::reset() { this->active = false; }
void PowerUp::collect() { this->active = false; }