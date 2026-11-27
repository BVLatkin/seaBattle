// ai.h — компьютерный противник.
// Интерфейс задан Участником 1, реализацию (ai.cpp) пишет Участник 2
// в ветке feature/ai. Публичные методы не менять — их вызывает Game.

#ifndef SEABATTLE_AI_H_
#define SEABATTLE_AI_H_

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
  // Приватные поля и вспомогательные методы добавляет Участник 2.
};

#endif  // SEABATTLE_AI_H_
