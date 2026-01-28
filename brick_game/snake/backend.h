#pragma once

#include "../common/common_backend.h"

#ifdef __cplusplus
extern "C" {
#endif

void snake_init_game_info(GameInfo_t *game_info);
void snake_userInput(UserAction_t action, bool hold);
GameInfo_t snake_updateCurrentState(void);

#ifdef __cplusplus
}
#endif