#pragma once

#include <SFML/Graphics.hpp>

class PowerUp {
public:
    PowerUp();

    [[nodiscard]] bool isActive() const;
    [[nodiscard]] sf::Sprite getSprite() const;
    [[nodiscard]] sf::FloatRect getBounds() const;

    void update(float dt, float speed);
    void spawn(int randomValue);
    void reset();
    void collect();

private:
    sf::Texture texture;
    sf::Sprite sprite;
    bool active;
};