#pragma once

constexpr int WINDOW_SIZE = 800;

// Rozne stany, w ktorych moze znajdowac sie gra.
enum class GameState {
  StartingMenu,
  Playing,
  RankingList,
  GameOver,
  NicknameInput
};
