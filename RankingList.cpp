#include "RankingList.h"

#include <iostream>

RankingList::RankingList() : db(nullptr) {
  int exit = sqlite3_open("ranking.db", &db);
  if (exit != SQLITE_OK) {
    db = nullptr;
  } else {
    initializeDatabase();
    loadFromDatabase();
  }
}

RankingList::~RankingList() {
  if (db) {
    sqlite3_close(db);
  }
}

void RankingList::initializeDatabase() {
  if (!db) {
    return;
  }

  const char *createTableSQL =
      "CREATE TABLE IF NOT EXISTS ranking ("
      "nickname TEXT PRIMARY KEY, "
      "score INTEGER NOT NULL);";

  char *errorMessage = nullptr;
  int exit = sqlite3_exec(db, createTableSQL, nullptr, nullptr, &errorMessage);
  if (exit != SQLITE_OK) {
    sqlite3_free(errorMessage);
  }
}

void RankingList::loadFromDatabase() {
  if (!db) {
    return;
  }

  list.clear();
  const char *selectSQL =
      "SELECT score, nickname FROM ranking ORDER BY score DESC LIMIT 100;";
  sqlite3_stmt *stmt;

  int exit = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr);
  if (exit == SQLITE_OK) {
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      int score = sqlite3_column_int(stmt, 0);
      const unsigned char *nickname = sqlite3_column_text(stmt, 1);
      list.push_back({score, reinterpret_cast<const char *>(nickname)});
    }
    sqlite3_finalize(stmt);
  }
}

std::vector<std::pair<int, std::string>> &RankingList::getList() {
  return list;
}

const std::vector<std::pair<int, std::string>> &RankingList::getList() const {
  return list;
}

void RankingList::update(Score &score, User &user) {
  if (!db) {
    return;
  }

  const std::string &nickname = user.getName();
  int newScore = score.getScore();

  const char *selectSQL = "SELECT score FROM ranking WHERE nickname = ?;";
  sqlite3_stmt *selectStmt;
  int currentBest = -1;

  if (sqlite3_prepare_v2(db, selectSQL, -1, &selectStmt, nullptr) ==
      SQLITE_OK) {
    sqlite3_bind_text(selectStmt, 1, nickname.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(selectStmt) == SQLITE_ROW) {
      currentBest = sqlite3_column_int(selectStmt, 0);
    }
    sqlite3_finalize(selectStmt);
  }

  if (newScore > currentBest) {
    const char *insertOrReplaceSQL =
        "INSERT OR REPLACE INTO ranking (nickname, score) VALUES (?, ?);";
    sqlite3_stmt *insertStmt;
    if (sqlite3_prepare_v2(db, insertOrReplaceSQL, -1, &insertStmt, nullptr) ==
        SQLITE_OK) {
      sqlite3_bind_text(insertStmt, 1, nickname.c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_int(insertStmt, 2, newScore);
      sqlite3_step(insertStmt);
      sqlite3_finalize(insertStmt);
    }
  }

  loadFromDatabase();
}