// ai.cpp — реализация компьютерного противника.

#include "ai.h"

#include <algorithm>
#include <stdexcept>

Ai::Ai(unsigned seed) : rng_(seed) {}

bool Ai::IsShot(Coord coord) const {
  return shots_[coord.row][coord.col];
}

void Ai::AddTarget(Coord coord) {
  if (!Board::InBounds(coord.row, coord.col)) return;
  if (IsShot(coord)) return;

  for (const Coord& target : targets_) {
    if (target.row == coord.row && target.col == coord.col) {
      return;
    }
  }

  targets_.push_back(coord);
}

Coord Ai::NextShot() {
  while (!targets_.empty()) {
    Coord target = targets_.back();
    targets_.pop_back();

    if (!IsShot(target)) {
      return target;
    }
  }

  std::vector<Coord> available;

  for (int row = 0; row < Board::kSize; ++row) {
    for (int col = 0; col < Board::kSize; ++col) {
      if (!shots_[row][col]) {
        available.push_back({row, col});
      }
    }
  }

  if (available.empty()) {
    throw std::runtime_error("Нет доступных клеток для выстрела.");
  }

  std::uniform_int_distribution<int> dist(
      0, static_cast<int>(available.size()) - 1);

  return available[dist(rng_)];
}

void Ai::ReportResult(Coord shot, ShotResult result) {
  if (!Board::InBounds(shot.row, shot.col)) return;

  shots_[shot.row][shot.col] = true;

  if (result == ShotResult::kHit) {
    hits_.push_back(shot);

    AddTarget({shot.row - 1, shot.col});
    AddTarget({shot.row + 1, shot.col});
    AddTarget({shot.row, shot.col - 1});
    AddTarget({shot.row, shot.col + 1});
  }

  if (result == ShotResult::kKill) {
    hits_.clear();
    targets_.clear();
  }
}