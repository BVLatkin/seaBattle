// board_test.cpp — проверка логики поля без ИИ и интерфейса.
// Ответственный: Участник 1 (ветка feature/core).
// Запуск: ./board_test — при успехе печатает "Все тесты пройдены".

#include <cassert>
#include <iostream>
#include <random>

#include "board.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  // 1. Корабли не могут касаться друг друга, даже углами.
  Board board;
  assert(board.PlaceShip(0, 0, 4, true));   // A1-D1
  assert(!board.PlaceShip(1, 4, 1, true));  // E2 касается углом
  assert(!board.PlaceShip(0, 7, 4, true));  // выходит за край
  assert(board.PlaceShip(2, 0, 1, true));   // A3 — можно

  // 2. Выстрелы: мимо, ранил, повтор, убил.
  assert(board.Shoot(5, 5) == ShotResult::kMiss);
  assert(board.Shoot(5, 5) == ShotResult::kAlreadyShot);
  assert(board.Shoot(0, 0) == ShotResult::kHit);
  assert(board.Shoot(0, 1) == ShotResult::kHit);
  assert(board.Shoot(0, 2) == ShotResult::kHit);
  assert(board.Shoot(0, 3) == ShotResult::kKill);
  // Клетки вокруг убитого корабля помечены промахами.
  assert(board.CellAt(1, 4) == Cell::kMiss);
  assert(board.Shoot(1, 4) == ShotResult::kAlreadyShot);
  assert(board.Shoot(-1, 3) == ShotResult::kInvalid);

  // 3. Конец игры.
  assert(!board.AllSunk());
  assert(board.Shoot(2, 0) == ShotResult::kKill);
  assert(board.AllSunk());

  // 4. Случайная расстановка: 10 кораблей, 20 палуб.
  std::mt19937 rng(42);
  for (int i = 0; i < 100; ++i) {
    Board random_board;
    random_board.PlaceRandomly(rng);
    assert(random_board.ships().size() == 10);
    int decks = 0;
    for (int r = 0; r < Board::kSize; ++r)
      for (int c = 0; c < Board::kSize; ++c)
        if (random_board.CellAt(r, c) == Cell::kShip) ++decks;
    assert(decks == 20);
  }

  assert(Board::CoordToString({9, 9}) == "J10");

  std::cout << "Все тесты пройдены" << std::endl;
  return 0;
}
