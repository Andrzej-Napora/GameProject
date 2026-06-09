#include "GameWindow.h"

namespace {
constexpr char WINDOW_TITLE[] = "Flappy Bird - Menu";
constexpr char BACKGROUND_TEXTURE_PATH[] = "resources/textures/background.png";
} // namespace

GameWindow::GameWindow() : width(WINDOW_SIZE), height(WINDOW_SIZE) {
  backgroundTexture.loadFromFile(BACKGROUND_TEXTURE_PATH);
  backgroundSprite.setTexture(backgroundTexture);
  backgroundSprite.setOrigin(0.f, 0.f);
  window.create(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), WINDOW_TITLE);
  window.setFramerateLimit(60);
}

unsigned int GameWindow::getWidth() const { return width; }
unsigned int GameWindow::getHeight() const { return height; }
sf::RenderWindow &GameWindow::getWindow() { return window; }
sf::Sprite GameWindow::getSprite() const { return backgroundSprite; }

void GameWindow::setWidth(unsigned int newWidth) { width = newWidth; }
void GameWindow::setHeight(unsigned int newHeight) { height = newHeight; }

void GameWindow::setWindow(unsigned int newWidth, unsigned int newHeight) {
  width = newWidth;
  height = newHeight;

  window.create(sf::VideoMode(width, height), WINDOW_TITLE);
  window.setFramerateLimit(60);
}
