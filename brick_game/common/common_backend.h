#ifndef BRICKGAME_LIBRARY_H
#define BRICKGAME_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define FIELD_W 10
#define FIELD_H 20

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

int load_high_score(const char *game_name);
void save_high_score(const char *game_name, int high_score);
int **create_matrix(int N, int M);
void free_matrix(int N, int **matrix);

#ifdef __cplusplus
}
#endif

#endif  // BRICKGAME_LIBRARY_H
