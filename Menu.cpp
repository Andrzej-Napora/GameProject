#include "Menu.h"

#include <algorithm>

namespace {
constexpr char MENU_BACKGROUND_PATH[] = "resources/textures/menu_bg.png";
}

Menu::Menu(const std::string &menuTitle, int optionsSize,
           const std::vector<std::string> &labels, sf::Font &fontArg)
    : font(fontArg) {
  constexpr float startingPositionMain = 250.f;

  if (backgroundTexture.loadFromFile(MENU_BACKGROUND_PATH)) {
    backgroundSprite.setTexture(backgroundTexture);
    hasBackground = true;

    const sf::Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(800.f / textureSize.x, 800.f / textureSize.y);
  }

  selector.setSize({500.f, 60.f});
  selector.setFillColor(sf::Color(255, 255, 255, 100));
  selector.setOrigin(250.f, 30.f);

  setupTitle(menuTitle, 50, activeColor, 50.f, 3.f);

  for (int i = 0; i < optionsSize && i < static_cast<int>(labels.size()); ++i) {
    const float yPosition = startingPositionMain + (100.f * i);
    options.push_back(
        createCenteredText(labels[i], 50, inactiveColor, yPosition));
    selectorPositions.push_back(yPosition);
  }

  menuCount = static_cast<int>(options.size());
}

Menu::Menu(const std::string &menuTitle, int optionsSize,
           const std::vector<std::pair<int, std::string>> &labels,
           sf::Font &fontArg)
    : font(fontArg) {
  setupTitle(menuTitle, 50, activeColor, 50.f, 2.f);

  const int safeOptionsSize =
      std::min(optionsSize, static_cast<int>(labels.size()));
  int filledCount = 0;

  for (int i = 0; i < safeOptionsSize; ++i) {
    if (labels[i].second.empty()) {
      continue;
    }

    const std::string line =
        std::to_string(labels[i].first) + " " + labels[i].second;
    const float yPosition = STARTING_POSITION + (ITEM_HEIGHT * filledCount);
    options.push_back(
        createCenteredText(line, 35, sf::Color::White, yPosition));
    selectorPositions.push_back(yPosition);
    ++filledCount;
  }

  menuCount = static_cast<int>(options.size());
  refreshScrollLimit();
}

Menu::Menu(const std::string &menuTitle, sf::Font &fontArg) : font(fontArg) {
  setupTitle(menuTitle, 70, sf::Color::Red, 200.f, 0.f);
}

void Menu::optionsUpdate(const std::vector<std::pair<int, std::string>> &list) {
  options.clear();
  selectorPositions.clear();
  scrollOffset = 0.f;

  int filledCount = 0;
  for (const auto &[score, nickname] : list) {
    if (nickname.empty()) {
      continue;
    }

    const std::string line = std::to_string(score) + " " + nickname;
    const float yPosition = STARTING_POSITION + (ITEM_HEIGHT * filledCount);
    options.push_back(
        createCenteredText(line, 35, sf::Color::White, yPosition));
    selectorPositions.push_back(yPosition);
    ++filledCount;
  }

  menuCount = static_cast<int>(options.size());
  refreshScrollLimit();
}

void Menu::updateSelection(int selectedIndex) {
  for (int i = 0; i < static_cast<int>(options.size()); ++i) {
    options[i].setFillColor(i == selectedIndex ? activeColor : inactiveColor);
    options[i].setCharacterSize(i == selectedIndex ? 52 : 50);

    const sf::FloatRect frame = options[i].getLocalBounds();
    options[i].setOrigin(frame.left + frame.width / 2.f,
                         frame.top + frame.height / 2.f);
  }
}

void Menu::drawBackground(sf::RenderWindow &window, int selectedIndex) {
  if (hasBackground) {
    window.draw(backgroundSprite);
  } else {
    window.clear(sf::Color(25, 25, 35));
  }

  updateSelection(selectedIndex);
  window.draw(title);
  for (const auto &option : options) {
    window.draw(option);
  }
}

void Menu::scroll(float delta) {
  constexpr float scrollSpeed = 20.f;
  const float newOffset =
      std::clamp(scrollOffset - (delta * scrollSpeed), 0.f, maxScroll);
  const float difference = newOffset - scrollOffset;
  scrollOffset = newOffset;

  for (auto &text : options) {
    text.move({0.f, -difference});
  }
}

void Menu::draw(sf::RenderWindow &window) {
  window.draw(title);

  for (const auto &text : options) {
    const float y = text.getPosition().y;
    if (y >= STARTING_POSITION - 10.f &&
        y <= STARTING_POSITION + VISIBLE_HEIGHT) {
      window.draw(text);
    }
  }
}

int Menu::getMenuCount() const { return menuCount; }
const sf::RectangleShape &Menu::getSelector() const { return selector; }
sf::RectangleShape &Menu::getSelectorMod() { return selector; }
const sf::Text &Menu::getTitle() const { return title; }
const std::vector<sf::Text> &Menu::getOptions() const { return options; }
const std::vector<float> &Menu::getSelectorPositions() const {
  return selectorPositions;
}

void Menu::setupTitle(const std::string &menuTitle, unsigned int characterSize,
                      sf::Color fillColor, float yPosition,
                      float outlineThickness) {
  title.setFont(font);
  title.setString(menuTitle);
  title.setCharacterSize(characterSize);
  title.setFillColor(fillColor);
  title.setOutlineColor(sf::Color::Black);
  title.setOutlineThickness(outlineThickness);

  const sf::FloatRect titleFrame = title.getLocalBounds();
  title.setOrigin(titleFrame.left + titleFrame.width / 2.f,
                  titleFrame.top + titleFrame.height / 2.f);
  title.setPosition({400.f, yPosition});
}

sf::Text Menu::createCenteredText(const std::string &textValue,
                                  unsigned int characterSize,
                                  sf::Color fillColor, float yPosition) const {
  sf::Text text(textValue, font, characterSize);
  text.setFillColor(fillColor);
  text.setOutlineColor(outlineColor);
  text.setOutlineThickness(characterSize >= 50 ? 2.f : 1.5f);

  const sf::FloatRect frame = text.getLocalBounds();
  text.setOrigin(frame.left + frame.width / 2.f,
                 frame.top + frame.height / 2.f);
  text.setPosition({400.f, yPosition});
  return text;
}

void Menu::refreshScrollLimit() {
  const float totalHeight = static_cast<float>(menuCount) * ITEM_HEIGHT;
  maxScroll = std::max(0.f, totalHeight - VISIBLE_HEIGHT);
}
