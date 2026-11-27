// display.h — вывод в консоль и ввод игрока.
// Интерфейс задан Участником 1, реализацию (display.cpp) пишет Участник 3
// в ветке feature/ui. Сигнатуры не менять — их вызывает Game.

#ifndef SEABATTLE_DISPLAY_H_
#define SEABATTLE_DISPLAY_H_

#include <string>

#include "board.h"

namespace ui {

// Печатает два поля рядом: своё (корабли видны) и поле противника
// (видны только попадания и промахи).
void PrintBoards(const Board& player, const Board& enemy);

// Читает ход игрока вида "B7" или "b7". При ошибке ввода просит повторить.
// Возвращает false, если игрок ввёл "q" (выход из игры).
bool ReadShot(Coord* shot);

// Выводит сообщение на отдельной строке.
void ShowMessage(const std::string& message);

// Задаёт вопрос и ждёт ответа y/n. Возвращает true для "y".
bool AskYesNo(const std::string& question);

}  // namespace ui

#endif  // SEABATTLE_DISPLAY_H_
