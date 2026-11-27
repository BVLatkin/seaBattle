// display.cpp — реализация консольного вывода и ввода игрока.
// Ответственный: Участник 3 (ветка feature/ui).

#include "display.h"

#include <cctype>
#include <iostream>
#include <string>

namespace ui {

namespace {

// Символ для отображения клетки своего поля.
char CellSymbol(Cell cell) {
  switch (cell) {
    case Cell::kShip:  return '#';   // корабль
    case Cell::kHit:   return 'X';   // попадание
    case Cell::kMiss:  return '.';   // промах
    case Cell::kEmpty: return '~';   // пусто
  }
  return '?';
}

// Символ для отображения клетки поля противника:
// корабли противника скрыты, показываем только результаты выстрелов.
char EnemyCellSymbol(Cell cell) {
  switch (cell) {
    case Cell::kShip:  return '~';   // скрыт
    case Cell::kHit:   return 'X';
    case Cell::kMiss:  return '.';
    case Cell::kEmpty: return '~';
  }
  return '?';
}

// Печатает одну строку двух полей рядом.
void PrintRow(const Board& player, const Board& enemy, int row) {
  // Номер строки.
  std::cout << (row + 1 < 10 ? " " : "") << (row + 1) << " ";

  // Своё поле.
  for (int c = 0; c < Board::kSize; ++c) {
    std::cout << CellSymbol(player.CellAt(row, c)) << ' ';
  }

  std::cout << "   ";  // разделитель

  // Поле противника.
  for (int c = 0; c < Board::kSize; ++c) {
    std::cout << EnemyCellSymbol(enemy.CellAt(row, c)) << ' ';
  }
  std::cout << '\n';
}

}  // namespace

void PrintBoards(const Board& player, const Board& enemy) {
  std::cout << '\n';

  // Шапка с буквами A–J для обоих полей.
  std::cout << "   ";
  for (int c = 0; c < Board::kSize; ++c) {
    std::cout << static_cast<char>('A' + c) << ' ';
  }
  std::cout << "     ";
  for (int c = 0; c < Board::kSize; ++c) {
    std::cout << static_cast<char>('A' + c) << ' ';
  }
  std::cout << '\n';

  // Строки полей.
  for (int r = 0; r < Board::kSize; ++r) {
    PrintRow(player, enemy, r);
  }

  std::cout << '\n';
  std::cout << "Ваше поле                                Поле противника\n";
  std::cout << "Обозначения: # корабль, X попадание, . промах, ~ пусто\n\n";
}

bool ReadShot(Coord* shot) {
  while (true) {
    std::cout << "Ваш ход (например B7, q — выход): ";
    std::string input;
    if (!std::getline(std::cin, input)) {
      // Поток закрыт — считаем, что игрок вышел.
      return false;
    }

    // Убираем пробелы по краям.
    size_t begin = input.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) {
      std::cout << "Пустой ввод, повторите ввод.\n";
      continue;
    }
    size_t end = input.find_last_not_of(" \t\r\n");
    std::string token = input.substr(begin, end - begin + 1);

    if (token == "q" || token == "Q") {
      return false;
    }

    // Формат: буква (A–J/a–j) + число (1–10).
    if (token.size() < 2) {
      std::cout << "Неверный формат, повторите ввод.\n";
      continue;
    }

    char letter = token[0];
    if (!std::isalpha(static_cast<unsigned char>(letter))) {
      std::cout << "Неверный формат, повторите ввод.\n";
      continue;
    }
    int col = std::toupper(static_cast<unsigned char>(letter)) - 'A';
    if (col < 0 || col >= Board::kSize) {
      std::cout << "Неверный столбец, повторите ввод.\n";
      continue;
    }

    std::string digits = token.substr(1);
    for (char ch : digits) {
      if (!std::isdigit(static_cast<unsigned char>(ch))) {
        std::cout << "Неверный формат, повторите ввод.\n";
        break;
      }
    }

    int number = 0;
    try {
      number = std::stoi(digits);
    } catch (...) {
      std::cout << "Неверный формат, повторите ввод.\n";
      continue;
    }
    if (number < 1 || number > Board::kSize) {
      std::cout << "Неверный номер строки, повторите ввод.\n";
      continue;
    }

    shot->row = number - 1;
    shot->col = col;
    return true;
  }
}

void ShowMessage(const std::string& message) {
  std::cout << message << '\n';
}

bool AskYesNo(const std::string& question) {
  while (true) {
    std::cout << question << " (y/n): ";
    std::string answer;
    if (!std::getline(std::cin, answer)) return false;
    if (answer == "y" || answer == "Y") return true;
    if (answer == "n" || answer == "N") return false;
    std::cout << "Пожалуйста, ответьте y или n.\n";
  }
}

}  // namespace ui