#pragma once

#include "GameConstants.h"

#include <SFML/Graphics.hpp>

class GameWindow {
public:
  GameWindow();

  [[nodiscard]] unsigned int getWidth() const;
  [[nodiscard]] unsigned int getHeight() const;
  [[nodiscard]] sf::Sprite getSprite() const;
  [[nodiscard]] sf::RenderWindow &getWindow();

  void setWidth(unsigned int width);
  void setHeight(unsigned int height);
  void setWindow(unsigned int width, unsigned int height);

private:
  sf::RenderWindow window;
  unsigned int width;
  unsigned int height;
  sf::Texture backgroundTexture;
  sf::Sprite backgroundSprite;
};
