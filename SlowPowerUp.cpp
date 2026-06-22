#include "SlowPowerUp.h"

namespace {
    constexpr char SLOW_TEXTURE_PATH[] = "resources/textures/powerup.png";
}

SlowPowerUp::SlowPowerUp() {
    this->texture.loadFromFile(SLOW_TEXTURE_PATH);
    this->sprite.setTexture(this->texture);

    const sf::Vector2u textureSize = this->texture.getSize();
    this->sprite.setScale(90.f / textureSize.x, 90.f / textureSize.y);

    const sf::FloatRect bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void SlowPowerUp::spawn(int randomValue) {
    if(this->active) return;

    if(randomValue > 50) {
        this->active = true;
        const float yPos = 150.f + 4.f * static_cast<float>(randomValue);
        this->sprite.setPosition({900.f, yPos});
    }
}