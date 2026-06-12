#include "PowerUp.h"

namespace {
    constexpr char POWERUP_TEXTURE_PATH[] = "resources/textures/powerup.png";
}

PowerUp::PowerUp() : active(false) {
    texture.loadFromFile(POWERUP_TEXTURE_PATH);
    sprite.setTexture(texture);

    const sf::Vector2u textureSize = texture.getSize();
    sprite.setScale(90.f / textureSize.x, 90.f / textureSize.y);

    const sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

bool PowerUp::isActive() const { return active; }
sf::Sprite PowerUp::getSprite() const { return sprite; }
sf::FloatRect PowerUp::getBounds() const { return sprite.getGlobalBounds(); }

void PowerUp::update(float dt, float speed) {
    if (active) {
        sprite.move(speed * dt, 0.f);
        if (sprite.getPosition().x < -100.f) {
            active = false;
        }
    }
}

void PowerUp::spawn(int randomValue) {
    if (!active) {
        active = true;
        const float yPos = 150.f + 4.f * static_cast<float>(randomValue);
        sprite.setPosition({900.f, yPos});
    }
}

void PowerUp::reset() { active = false; }

void PowerUp::collect() { active = false; }