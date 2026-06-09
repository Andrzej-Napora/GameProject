#pragma once

#include "Score.h"
#include "User.h"
#include "sqlite/sqlite3.h"

#include <string>
#include <utility>
#include <vector>

class RankingList {
public:
  RankingList();
  ~RankingList();

  [[nodiscard]] std::vector<std::pair<int, std::string>> &getList();
  [[nodiscard]] const std::vector<std::pair<int, std::string>> &getList() const;

  void update(Score &score, User &user);

private:
  void loadFromDatabase();
  void initializeDatabase();

  static constexpr short LIST_SIZE = 100;
  std::vector<std::pair<int, std::string>> list;
  sqlite3 *db;
};