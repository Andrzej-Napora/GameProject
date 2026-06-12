#include "Bird.h"
#include "GameConstants.h"
#include "GameWindow.h"
#include "Menu.h"
#include "Obstacles.h"
#include "PowerUp.h"
#include "RankingList.h"
#include "Score.h"
#include "User.h"
#include "Vortex.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>
#include <string>
#include <vector>

namespace {
constexpr char MAIN_FONT_PATH[] = "resources/font/Cabin-SemiBold.ttf";

bool isTopMenuInput(const sf::Event &event) {
  return event.type == sf::Event::KeyPressed &&
         (event.key.code == sf::Keyboard::W ||
          event.key.code == sf::Keyboard::Up);
}

bool isBottomMenuInput(const sf::Event &event) {
  return event.type == sf::Event::KeyPressed &&
         (event.key.code == sf::Keyboard::S ||
          event.key.code == sf::Keyboard::Down);
}

bool isConfirmInput(const sf::Event &event) {
  return event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Enter;
}

bool isBackInput(const sf::Event &event) {
  return event.type == sf::Event::KeyPressed &&
         event.key.code == sf::Keyboard::Escape;
}

bool isRestartInput(const sf::Event &event) {
  return (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Space) ||
         (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left);
}

bool shouldSaveScore(const Score &score) {
  return score.getScore() > 0;
}
}

int main() {
  std::random_device randomDevice;
  std::mt19937 generator(randomDevice());
  std::uniform_int_distribution<int> distribution(0, 100);
  int randomValue = distribution(generator);

  sf::Font mainFont;
  mainFont.loadFromFile(MAIN_FONT_PATH);

  GameWindow gameWindow;
  User user(mainFont);
  RankingList rankingList;
  Menu startingMenu("FLAPPY BIRD", 4,
                    {"Graj", "Ranking", "Zmien nick", "Wyjscie"}, mainFont);
  Menu gameOverMenu("Koniec gry", mainFont);
  Menu rankingMenu("Ranking", static_cast<int>(rankingList.getList().size()),
                   rankingList.getList(), mainFont);
  GameState currentGameState = GameState::NicknameInput;
  Bird bird;
  ObstacleQueue obstacleQueue;
  Score score;
  Vortex vortex(randomValue);
  PowerUp powerUp;

  rankingMenu.optionsUpdate(rankingList.getList());

  int selectedIndex = 0;
  bool nicknameSet = false;
  bool isSlowed = false;
  int slowObstaclesRemaining = 0;

  sf::RectangleShape slowEffectTint;
  slowEffectTint.setSize({800.f, 800.f});
  slowEffectTint.setFillColor(sf::Color(0, 255, 255, 40));

  sf::Clock clock;

  while (gameWindow.getWindow().isOpen()) {
    float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);

    sf::Event event;
    while (gameWindow.getWindow().pollEvent(event)) {
      const GameState stateBeforeEvent = currentGameState;

      if (event.type == sf::Event::Closed) {
        gameWindow.getWindow().close();
      }

      if (stateBeforeEvent == GameState::StartingMenu) {
        if (isConfirmInput(event)) {
          switch (selectedIndex) {
          case 0:
            currentGameState = GameState::Playing;
            break;
          case 1:
            currentGameState = GameState::RankingList;
            break;
          case 2:
            currentGameState = GameState::NicknameInput;
            break;
          case 3:
            return 0;
          default:
            break;
          }
        } else if (isTopMenuInput(event)) {
          selectedIndex = selectedIndex <= 0 ? startingMenu.getMenuCount() - 1
                                             : selectedIndex - 1;
        } else if (isBottomMenuInput(event)) {
          selectedIndex = selectedIndex >= startingMenu.getMenuCount() - 1
                              ? 0
                              : selectedIndex + 1;
        }
      }

      if (stateBeforeEvent == GameState::RankingList) {
        if (isBackInput(event)) {
          currentGameState = GameState::StartingMenu;
        } else if (event.type == sf::Event::KeyPressed &&
                   event.key.code == sf::Keyboard::Up) {
          rankingMenu.scroll(1.f);
        } else if (event.type == sf::Event::KeyPressed &&
                   event.key.code == sf::Keyboard::Down) {
          rankingMenu.scroll(-1.f);
        } else if (event.type == sf::Event::MouseWheelScrolled &&
                   event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
          rankingMenu.scroll(event.mouseWheelScroll.delta);
        }
      }

      if (stateBeforeEvent == GameState::Playing) {
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Space) {
            bird.jumpHandle();
          } else if (event.key.code == sf::Keyboard::A ||
                     event.key.code == sf::Keyboard::Left) {
            bird.leftMovementHandle();
          } else if (event.key.code == sf::Keyboard::D ||
                     event.key.code == sf::Keyboard::Right) {
            bird.rightMovementHandle();
          }
        } else if (event.type == sf::Event::MouseButtonPressed &&
                   event.mouseButton.button == sf::Mouse::Left) {
          bird.jumpHandle();
        }
      }

      if (stateBeforeEvent == GameState::GameOver && isRestartInput(event)) {
        score.setScore(0);
        currentGameState = GameState::StartingMenu;
      }

      if (stateBeforeEvent == GameState::NicknameInput) {
        if (event.type == sf::Event::TextEntered && event.text.unicode < 128) {
          if (event.text.unicode == 8) {
            if (!user.getName().empty()) {
              user.getName().pop_back();
            }
          } else if (event.text.unicode >= 32) {
            user.getName() += static_cast<char>(event.text.unicode);
          }
        }

        if (isConfirmInput(event)) {
          if (!user.getName().empty()) {
            nicknameSet = true;
            currentGameState = GameState::StartingMenu;
          }
        }
      }
    }

    gameWindow.getWindow().clear();

    if (currentGameState == GameState::StartingMenu) {
      startingMenu.getSelectorMod().setPosition(
          400.f, startingMenu.getSelectorPositions()[selectedIndex]);
      startingMenu.drawBackground(gameWindow.getWindow(), selectedIndex);
      gameWindow.getWindow().draw(startingMenu.getSelector());
    } else if (currentGameState == GameState::RankingList) {
      rankingMenu.draw(gameWindow.getWindow());
    } else if (currentGameState == GameState::NicknameInput) {
      user.updateNameText();
      const std::string promptText =
          nicknameSet ? "Wprowadz nowy nick" : "Podaj nick";
      sf::Text prompt(promptText, mainFont, 40);
      prompt.setPosition({60.f, 100.f});
      gameWindow.getWindow().draw(prompt);
      gameWindow.getWindow().draw(user.getNameText());
    } else if (currentGameState == GameState::GameOver) {
      gameWindow.getWindow().draw(gameOverMenu.getTitle());
    } else if (currentGameState == GameState::Playing) {
      bird.update(deltaTime, vortex.calculateGravity(bird));

      randomValue = distribution(generator);
      obstacleQueue.spawnObstacleIfNeeded(deltaTime, randomValue);
      obstacleQueue.removeOutOfScreenObstacles(deltaTime);
      vortex.update(deltaTime, randomValue);
      powerUp.update(deltaTime, obstacleQueue.getCurrentSpeed());

      const sf::FloatRect birdFrame = bird.getBirdShape().getGlobalBounds();

      if (powerUp.isActive() && birdFrame.intersects(powerUp.getBounds())) {
        powerUp.collect();
        isSlowed = true;
        slowObstaclesRemaining = 5;
        obstacleQueue.updateSpeed(score.getScore(), isSlowed);
      }

      for (auto &obstacle : obstacleQueue.getQueue()) {
        obstacle.getObstacles().first.update(deltaTime);
        obstacle.getObstacles().second.update(deltaTime);
      }

      const float birdX = bird.getXPosition();
      for (auto &obstacle : obstacleQueue.getQueue()) {
        if (!obstacle.isPassed() &&
            birdX > obstacle.getObstacles().first.getXPosition() +
                        obstacle.getObstacles().first.getXSize()) {
          obstacle.setPassed(true);
          score.increment();

          if (isSlowed) {
            slowObstaclesRemaining--;
            if (slowObstaclesRemaining <= 0) {
              isSlowed = false;
            }
          }

          obstacleQueue.updateSpeed(score.getScore(), isSlowed);

          if (score.getScore() % 10 == 0 && score.getScore() > 0 &&
              !isSlowed && !powerUp.isActive()) {
            powerUp.spawn(randomValue);
          }
        }
      }

      bool hasCollided = birdFrame.left < 0.f || birdFrame.left + birdFrame.width > 800.f ||
                         birdFrame.top < 0.f || birdFrame.top + birdFrame.height > 800.f;

      if (!hasCollided) {
        for (auto &obstacle : obstacleQueue.getQueue()) {
          const sf::FloatRect upperObstacleFrame =
              obstacle.getObstacles().first.getShape().getGlobalBounds();
          const sf::FloatRect bottomObstacleFrame =
              obstacle.getObstacles().second.getShape().getGlobalBounds();

          //if (birdFrame.intersects(upperObstacleFrame) ||
          //    birdFrame.intersects(bottomObstacleFrame)) {
          //  hasCollided = true;
          //  break;
          //}
        }
      }

      if (hasCollided) {
        bird.reset();
        obstacleQueue.reset();
        vortex.reset(randomValue);
        powerUp.reset();
        isSlowed = false;
        slowObstaclesRemaining = 0;

        if (shouldSaveScore(score)) {
          rankingList.update(score, user);
          rankingMenu.optionsUpdate(rankingList.getList());
        }

        currentGameState = GameState::GameOver;
      }

      gameWindow.getWindow().draw(gameWindow.getSprite());
      for (auto &obstacle : obstacleQueue.getQueue()) {
        gameWindow.getWindow().draw(obstacle.getObstacles().first.getShape());
        gameWindow.getWindow().draw(obstacle.getObstacles().second.getShape());
      }
      gameWindow.getWindow().draw(vortex.getShape());
      if (powerUp.isActive()) {
        gameWindow.getWindow().draw(powerUp.getSprite());
      }
      gameWindow.getWindow().draw(bird.getBirdShape());
      if (isSlowed) {
        gameWindow.getWindow().draw(slowEffectTint);
      }
      gameWindow.getWindow().draw(score.getText());
    }

    gameWindow.getWindow().display();
  }

  return 0;
}