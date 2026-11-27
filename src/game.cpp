// game.cpp — реализация игрового цикла.
// Ответственный: Участник 1 (ветка feature/core).

#include "game.h"

#include <string>

#include "display.h"

Game::Game(unsigned seed) : rng_(seed), ai_(seed + 1) {}

void Game::Run() {
  ui::ShowMessage("=== МОРСКОЙ БОЙ ===");
  ui::ShowMessage("Ход вводится как буква и число, например B7. Выход — q.");

  SetupPlayerFleet();
  enemy_board_.PlaceRandomly(rng_);

  bool player_turn = true;
  while (true) {
    if (player_turn) {
      bool quit = false;
      player_turn = PlayerTurn(&quit);
      if (quit) {
        ui::ShowMessage("Игра прервана.");
        return;
      }
      if (enemy_board_.AllSunk()) {
        ui::PrintBoards(player_board_, enemy_board_);
        ui::ShowMessage("ПОБЕДА! Все корабли противника потоплены.");
        return;
      }
    } else {
      player_turn = !ComputerTurn();
      if (player_board_.AllSunk()) {
        ui::PrintBoards(player_board_, enemy_board_);
        ui::ShowMessage("ПОРАЖЕНИЕ. Компьютер потопил весь ваш флот.");
        return;
      }
    }
  }
}

void Game::SetupPlayerFleet() {
  while (true) {
    player_board_.PlaceRandomly(rng_);
    ui::PrintBoards(player_board_, enemy_board_);
    if (ui::AskYesNo("Оставить такую расстановку кораблей?")) return;
  }
}

bool Game::PlayerTurn(bool* quit) {
  ui::PrintBoards(player_board_, enemy_board_);
  Coord shot{};
  if (!ui::ReadShot(&shot)) {
    *quit = true;
    return false;
  }

  std::string where = Board::CoordToString(shot);
  switch (enemy_board_.Shoot(shot.row, shot.col)) {
    case ShotResult::kInvalid:
      ui::ShowMessage("Такой клетки нет на поле.");
      return true;
    case ShotResult::kAlreadyShot:
      ui::ShowMessage("Вы уже стреляли в " + where + ".");
      return true;
    case ShotResult::kMiss:
      ui::ShowMessage(where + ": мимо. Ход переходит компьютеру.");
      return false;
    case ShotResult::kHit:
      ui::ShowMessage(where + ": ранил! Стреляйте ещё.");
      return true;
    case ShotResult::kKill:
      ui::ShowMessage(where + ": убил! Осталось кораблей противника: " +
                      std::to_string(enemy_board_.AliveShips()));
      return true;
  }
  return false;
}

bool Game::ComputerTurn() {
  // Защита от зацикливания, если ИИ выдаёт уже обстрелянные клетки.
  const int kMaxAttempts = 200;
  for (int attempt = 0; attempt < kMaxAttempts; ++attempt) {
    Coord shot = ai_.NextShot();
    ShotResult result = player_board_.Shoot(shot.row, shot.col);
    ai_.ReportResult(shot, result);
    if (result == ShotResult::kInvalid || result == ShotResult::kAlreadyShot) {
      continue;
    }

    std::string where = "Компьютер стреляет в " + Board::CoordToString(shot);
    if (result == ShotResult::kMiss) {
      ui::ShowMessage(where + ": мимо. Ваш ход.");
      return false;
    }
    if (result == ShotResult::kHit) {
      ui::ShowMessage(where + ": ранил ваш корабль!");
    } else {
      ui::ShowMessage(where + ": убил ваш корабль! Осталось ваших: " +
                      std::to_string(player_board_.AliveShips()));
    }
    return true;
  }
  return false;
}
