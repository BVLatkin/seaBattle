// main.cpp — точка входа.
// Ответственный: Участник 1 (ветка feature/core).

#include <ctime>

#include "game.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
  // Чтобы русский текст корректно выводился в консоли Windows.
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
#endif
  Game game(static_cast<unsigned>(std::time(nullptr)));
  game.Run();
  return 0;
}
