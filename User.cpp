#include "User.h"

User::User(sf::Font &font) : nickname("") {
  nameText.setCharacterSize(50.f);
  nameText.setFillColor(sf::Color::Cyan);
  nameText.setFont(font);
  updateNameText();
}

std::string &User::getName() { return nickname; }
sf::Text &User::getNameText() { return nameText; }

void User::setNickname(const std::string &name) {
  nickname = name;
  updateNameText();
}

void User::updateNameText() {
  nameText.setString(nickname);
  const sf::FloatRect nameFrame = nameText.getLocalBounds();
  nameText.setOrigin(nameFrame.left + nameFrame.width / 2.f,
                     nameFrame.top + nameFrame.height / 2.f);
  nameText.setPosition({400.f, 200.f});
}

void User::resetName() {
  nickname.clear();
  updateNameText();
}
