#include "../../brick_game/tetris/tetris_backend.h"
#include "frontend.h"

void run_tetris_cli() {
  run_game_cli(init_game_info, userInput, updateCurrentState, 0);
}
