#include "../../brick_game/snake/backend.h"
#include "frontend.h"

void run_snake_cli() {
  run_game_cli(snake_init_game_info, snake_userInput, snake_updateCurrentState,
               1);
}
