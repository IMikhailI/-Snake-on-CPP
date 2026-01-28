#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game/tetris/tetris_backend.h"

typedef void (*InitFn)(GameInfo_t *info);
typedef void (*InputFn)(UserAction_t action, bool hold);
typedef GameInfo_t (*UpdateFn)(void);

/**
 * @file frontend.h
 * @brief Терминальный интерфейс для Tetris (ncurses)
 */

/**
 * @struct FrontendContext
 * @brief Хранит окна ncurses для отрисовки поля и панели информации
 */
typedef struct {
  WINDOW *game_win; /**< окно для отрисовки игрового поля */
  WINDOW *info_win; /**< окно для отрисовки информационной панели */
} FrontendContext;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Инициализация ncurses и создание окон (game_win, info_win)
 * @return указатель на созданный FrontendContext
 */
FrontendContext *init_ncurses();

/**
 * @brief Отрисовка игрового поля и информационной панели
 * @param ctx контекст фронтенда (окна)
 * @param game_info данные об игре (поле, счет, уровень, рекорд и т.д.)
 */
void rendering(FrontendContext *ctx, GameInfo_t *game_info);

// Общий раннер для CLI-игры
static inline void run_game_cli(InitFn init_game_info, InputFn user_input,
                                UpdateFn update_state, int flag) {
  FrontendContext *ctx = init_ncurses();
  if (ctx != NULL) {
    srand(time(NULL));

    GameInfo_t game_info = {0};
    init_game_info(&game_info);

    bool run_game = true;
    bool hold = false;
    while (run_game) {
      int ch = getch();

      if (ch != ERR) {
        UserAction_t action = resolve_input(ch);
        if (action == Terminate) {
          run_game = false;
          continue;
        }
        if (action == Action && flag == 1) {
          if (!hold) {
            hold = true;
            user_input(Action, true);
          }
        } else {
          user_input(action, false);
        }
      } else if (hold) {
        hold = false;
        user_input(Action, false);
      }

      game_info = update_state();
      rendering(ctx, &game_info);
      napms(50);
    }

    endwin();
    free(ctx);
  }
}

#ifdef __cplusplus
}
#endif

#endif  // FRONTEND_H
