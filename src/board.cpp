// board.cpp — реализация игрового поля.
// Ответственный: Участник 1 (ветка feature/core).

#include "board.h"

Board::Board() { Clear(); }

void Board::Clear() {
  for (int r = 0; r < kSize; ++r) {
    for (int c = 0; c < kSize; ++c) {
      cells_[r][c] = Cell::kEmpty;
      ship_index_[r][c] = -1;
    }
  }
  ships_.clear();
}

bool Board::InBounds(int row, int col) {
  return row >= 0 && row < kSize && col >= 0 && col < kSize;
}

std::string Board::CoordToString(Coord coord) {
  std::string result(1, static_cast<char>('A' + coord.col));
  result += std::to_string(coord.row + 1);
  return result;
}

bool Board::CanPlace(int row, int col, int length, bool horizontal) const {
  if (length <= 0) return false;
  for (int i = 0; i < length; ++i) {
    int r = horizontal ? row : row + i;
    int c = horizontal ? col + i : col;
    if (!InBounds(r, c)) return false;
    // Проверяем саму клетку и всех 8 соседей.
    for (int dr = -1; dr <= 1; ++dr) {
      for (int dc = -1; dc <= 1; ++dc) {
        int nr = r + dr;
        int nc = c + dc;
        if (InBounds(nr, nc) && cells_[nr][nc] == Cell::kShip) return false;
      }
    }
  }
  return true;
}

bool Board::PlaceShip(int row, int col, int length, bool horizontal) {
  if (!CanPlace(row, col, length, horizontal)) return false;
  int index = static_cast<int>(ships_.size());
  ships_.push_back(Ship{row, col, length, horizontal, 0});
  for (int i = 0; i < length; ++i) {
    int r = horizontal ? row : row + i;
    int c = horizontal ? col + i : col;
    cells_[r][c] = Cell::kShip;
    ship_index_[r][c] = index;
  }
  return true;
}

void Board::PlaceRandomly(std::mt19937& rng) {
  const int kFleet[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
  std::uniform_int_distribution<int> pos(0, kSize - 1);
  std::uniform_int_distribution<int> dir(0, 1);

  while (true) {
    Clear();
    bool ok = true;
    for (int length : kFleet) {
      bool placed = false;
      for (int attempt = 0; attempt < 1000 && !placed; ++attempt) {
        placed = PlaceShip(pos(rng), pos(rng), length, dir(rng) == 1);
      }
      if (!placed) {  // Редкий тупик — начинаем расстановку заново.
        ok = false;
        break;
      }
    }
    if (ok) return;
  }
}

ShotResult Board::Shoot(int row, int col) {
  if (!InBounds(row, col)) return ShotResult::kInvalid;

  Cell& cell = cells_[row][col];
  if (cell == Cell::kMiss || cell == Cell::kHit) return ShotResult::kAlreadyShot;
  if (cell == Cell::kEmpty) {
    cell = Cell::kMiss;
    return ShotResult::kMiss;
  }

  cell = Cell::kHit;
  Ship& ship = ships_[ship_index_[row][col]];
  ++ship.hits;
  if (ship.IsSunk()) {
    MarkAroundShip(ship);
    return ShotResult::kKill;
  }
  return ShotResult::kHit;
}

void Board::MarkAroundShip(const Ship& ship) {
  for (int i = 0; i < ship.length; ++i) {
    int r = ship.horizontal ? ship.row : ship.row + i;
    int c = ship.horizontal ? ship.col + i : ship.col;
    for (int dr = -1; dr <= 1; ++dr) {
      for (int dc = -1; dc <= 1; ++dc) {
        int nr = r + dr;
        int nc = c + dc;
        if (InBounds(nr, nc) && cells_[nr][nc] == Cell::kEmpty) {
          cells_[nr][nc] = Cell::kMiss;
        }
      }
    }
  }
}

bool Board::AllSunk() const { return AliveShips() == 0; }

int Board::AliveShips() const {
  int alive = 0;
  for (const Ship& ship : ships_) {
    if (!ship.IsSunk()) ++alive;
  }
  return alive;
}
