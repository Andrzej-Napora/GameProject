#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class User {
public:
  explicit User(sf::Font &font);

  [[nodiscard]] std::string &getName();
  [[nodiscard]] sf::Text &getNameText();

  void setNickname(const std::string &name);
  void updateNameText();
  void resetName();

private:
  std::string nickname;
  sf::Text nameText;
};
