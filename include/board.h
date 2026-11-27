// board.h — игровое поле «Морского боя».
// Ответственный: Участник 1 (ветка feature/core).

#ifndef SEABATTLE_BOARD_H_
#define SEABATTLE_BOARD_H_

#include <random>
#include <string>
#include <vector>

// Координата клетки: строка 0..9 (цифры 1..10), столбец 0..9 (буквы A..J).
struct Coord {
  int row;
  int col;
};

// Корабль: начальная клетка, длина, направление и число попаданий.
struct Ship {
  int row;
  int col;
  int length;
  bool horizontal;
  int hits = 0;

  bool IsSunk() const { return hits >= length; }
};

// Состояние клетки поля.
enum class Cell { kEmpty, kShip, kMiss, kHit };

// Результат выстрела.
enum class ShotResult { kMiss, kHit, kKill, kAlreadyShot, kInvalid };

class Board {
 public:
  static constexpr int kSize = 10;

  Board();

  // Очищает поле и удаляет все корабли.
  void Clear();

  // Можно ли поставить корабль: внутри поля и не касается других кораблей
  // (даже по диагонали).
  bool CanPlace(int row, int col, int length, bool horizontal) const;

  // Ставит корабль, если это разрешено. Возвращает true при успехе.
  bool PlaceShip(int row, int col, int length, bool horizontal);

  // Случайная расстановка стандартного флота: 1x4, 2x3, 3x2, 4x1.
  void PlaceRandomly(std::mt19937& rng);

  // Выстрел по клетке. При потоплении корабля клетки вокруг него
  // автоматически помечаются как промахи.
  ShotResult Shoot(int row, int col);

  // Все ли корабли потоплены.
  bool AllSunk() const;

  // Сколько кораблей ещё на плаву.
  int AliveShips() const;

  Cell CellAt(int row, int col) const { return cells_[row][col]; }
  const std::vector<Ship>& ships() const { return ships_; }

  static bool InBounds(int row, int col);

  // Перевод координаты в строку вида "A1", "J10".
  static std::string CoordToString(Coord coord);

 private:
  void MarkAroundShip(const Ship& ship);

  Cell cells_[kSize][kSize];
  int ship_index_[kSize][kSize];  // Номер корабля в клетке или -1.
  std::vector<Ship> ships_;
};

#endif  // SEABATTLE_BOARD_H_
