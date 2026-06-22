#pragma once

#include <SFML/Graphics.hpp>

class PowerUp {
public:
    virtual ~PowerUp() = default;

    [[nodiscard]] bool isActive() const;
    [[nodiscard]] sf::Sprite getSprite() const;
    [[nodiscard]] sf::FloatRect getBounds() const;

    virtual void update(float dt, float speed);
    virtual void reset();
    virtual void collect();
    virtual void spawn(int randomValue) = 0;

protected:
    sf::Texture texture;
    sf::Sprite sprite;
    bool active = false;
};