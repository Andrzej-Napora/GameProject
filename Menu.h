#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <vector>

class Menu {
public:
  Menu(const std::string &title, int optionsSize,
       const std::vector<std::string> &labels, sf::Font &font);
  Menu(const std::string &title, int optionsSize,
       const std::vector<std::pair<int, std::string>> &labels, sf::Font &font);
  Menu(const std::string &title, sf::Font &font);

  [[nodiscard]] int getMenuCount() const;
  [[nodiscard]] const sf::RectangleShape &getSelector() const;
  [[nodiscard]] sf::RectangleShape &getSelectorMod();
  [[nodiscard]] const sf::Text &getTitle() const;
  [[nodiscard]] const std::vector<sf::Text> &getOptions() const;
  [[nodiscard]] const std::vector<float> &getSelectorPositions() const;

  void optionsUpdate(const std::vector<std::pair<int, std::string>> &list);
  void updateSelection(int selectedIndex);
  void drawBackground(sf::RenderWindow &window, int selectedIndex);
  void scroll(float delta);
  void draw(sf::RenderWindow &window);

private:
  void setupTitle(const std::string &title, unsigned int characterSize,
                  sf::Color fillColor, float yPosition, float outlineThickness);
  sf::Text createCenteredText(const std::string &text,
                              unsigned int characterSize, sf::Color fillColor,
                              float yPosition) const;
  void refreshScrollLimit();

  int menuCount = 0;
  sf::Font font;
  sf::RectangleShape selector;
  sf::Text title;
  std::vector<sf::Text> options;
  std::vector<float> selectorPositions;
  float scrollOffset = 0.f;
  float maxScroll = 0.f;

  static constexpr float STARTING_POSITION = 100.f;
  static constexpr float ITEM_HEIGHT = 40.f;
  static constexpr float VISIBLE_HEIGHT = 450.f;

  sf::Texture backgroundTexture;
  sf::Sprite backgroundSprite;
  bool hasBackground = false;

  const sf::Color activeColor = sf::Color(255, 204, 0);
  const sf::Color inactiveColor = sf::Color(180, 180, 180);
  const sf::Color outlineColor = sf::Color(0, 0, 0, 200);
};
