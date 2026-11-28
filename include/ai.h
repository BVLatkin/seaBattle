// ai.h — компьютерный противник.
// Интерфейс задан Участником 1, реализацию (ai.cpp) пишет Участник 2
// в ветке feature/ai. Публичные методы не менять — их вызывает Game.

#ifndef SEABATTLE_AI_H_
#define SEABATTLE_AI_H_

#include <random>
#include <vector>

#include "board.h"

class Ai {
 public:
  explicit Ai(unsigned seed);

  // Возвращает клетку, в которую компьютер стреляет следующей.
  // Не должна повторять клетки, по которым уже стреляли.
  Coord NextShot();

  // Сообщает компьютеру результат его выстрела, чтобы он
  // «добивал» раненый корабль и не стрелял рядом с убитыми.
  void ReportResult(Coord shot, ShotResult result);

 private:
  // Проверяет, стреляли ли уже в эту клетку.
  bool IsShot(Coord coord) const;

  // Добавляет соседнюю клетку в список целей.
  void AddTarget(Coord coord);

  // Генератор случайных чисел.
  std::mt19937 rng_;

  // Информация об уже обстрелянных клетках.
  bool shots_[Board::kSize][Board::kSize]{};

  // Координаты попаданий по раненым кораблям.
  std::vector<Coord> hits_;

  // Клетки, которые нужно проверить при добивании.
  std::vector<Coord> targets_;
};

#endif  // SEABATTLE_AI_H_