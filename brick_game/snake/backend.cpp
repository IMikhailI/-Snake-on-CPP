#include "backend.h"

#include <memory>

#include "snake_game.h"

static std::unique_ptr<s21::SnakeGame> game;

extern "C" void snake_init_game_info(GameInfo_t *game_info) {
  if (!game) game = std::make_unique<s21::SnakeGame>();
  game->init();
  *game_info = game->tick();
}

extern "C" void snake_userInput(UserAction_t action, bool hold) {
  if (game) game->handleInput(action, hold);
}

extern "C" GameInfo_t snake_updateCurrentState(void) {
  if (game) return game->tick();
  GameInfo_t empty{};
  return empty;
}