#pragma once

#include <SFML/Graphics.hpp>

class Score {
public:
  Score();

  [[nodiscard]] int getScore() const;
  [[nodiscard]] const sf::Text &getText() const;

  void setScore(int score);
  void increment();

private:
  unsigned int score;
  sf::Text scoreText;
  sf::Font font;
};