#include "Score.h"

#include <string>

namespace {
constexpr char SCORE_FONT_PATH[] = "resources/font/Cabin-SemiBold.ttf";
}

Score::Score() : score(0) {
  font.loadFromFile(SCORE_FONT_PATH);
  scoreText.setFont(font);
  scoreText.setString(std::to_string(score));
  scoreText.setCharacterSize(30);
  scoreText.setFillColor(sf::Color::Blue);
  scoreText.setPosition({700.f, 50.f});
}

int Score::getScore() const { return static_cast<int>(score); }
const sf::Text &Score::getText() const { return scoreText; }

void Score::setScore(int newScore) {
  score = static_cast<unsigned int>(newScore);
  scoreText.setString(std::to_string(score));
}

void Score::increment(float deltaTime) {
  score += static_cast<unsigned int>(60.f * deltaTime);
  scoreText.setString(std::to_string(score));
}
