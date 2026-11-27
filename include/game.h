// game.h — игровой цикл: игрок против компьютера.
// Ответственный: Участник 1 (ветка feature/core).

#ifndef SEABATTLE_GAME_H_
#define SEABATTLE_GAME_H_

#include <random>

#include "ai.h"
#include "board.h"

class Game {
 public:
  explicit Game(unsigned seed);

  // Запускает партию и возвращает управление после её окончания.
  void Run();

 private:
  void SetupPlayerFleet();
  // Ход игрока. Возвращает true, если игрок продолжает стрелять.
  // В *quit пишется true, если игрок вышел из игры.
  bool PlayerTurn(bool* quit);
  // Ход компьютера. Возвращает true, если компьютер стреляет ещё раз.
  bool ComputerTurn();

  std::mt19937 rng_;
  Board player_board_;
  Board enemy_board_;
  Ai ai_;
};

#endif  // SEABATTLE_GAME_H_
