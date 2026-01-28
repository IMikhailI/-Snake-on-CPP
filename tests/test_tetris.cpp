#include <gtest/gtest.h>

#include <cstring>

#include "brick_game/common/common_backend.h"
#include "brick_game/tetris/tetris_backend.h"

#ifndef KEY_LEFT
#define KEY_LEFT 0404
#endif
#ifndef KEY_RIGHT
#define KEY_RIGHT 0405
#endif
#ifndef KEY_DOWN
#define KEY_DOWN 0402
#endif
#ifndef KEY_UP
#define KEY_UP 0403
#endif

#define S() get_state()

TEST(TetrisBackendTest, ResolveInput) {
  EXPECT_EQ(resolve_input(KEY_LEFT), Left);
  EXPECT_EQ(resolve_input(KEY_RIGHT), Right);
  EXPECT_EQ(resolve_input(KEY_DOWN), Down);
  EXPECT_EQ(resolve_input(' '), Action);
  EXPECT_EQ(resolve_input('p'), Pause);
  EXPECT_EQ(resolve_input('P'), Pause);
  EXPECT_EQ(resolve_input('q'), Terminate);
  EXPECT_EQ(resolve_input('Q'), Terminate);
  EXPECT_EQ(resolve_input('x'), Start);
}

TEST(TetrisBackendTest, CreateMatrix) {
  int rows = 3, cols = 4;
  int** mat = create_matrix(rows, cols);
  ASSERT_NE(mat, nullptr);
  for (int i = 0; i < rows; i++) {
    ASSERT_NE(mat[i], nullptr);
    for (int j = 0; j < cols; j++) {
      EXPECT_EQ(mat[i][j], 0);
    }
  }
  free_matrix(rows, mat);
}

TEST(TetrisBackendTest, InitGameInfo) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  ASSERT_NE(game_info.field, nullptr);
  ASSERT_NE(game_info.next, nullptr);

  GameInfo_t updated = updateCurrentState();
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      EXPECT_TRUE((updated.field[i][j] == 0) || (updated.field[i][j] == 1) ||
                  (updated.field[i][j] == 2));
    }
  }
  EXPECT_EQ(updated.score, 0);
  EXPECT_GE(updated.high_score, 0);
  EXPECT_EQ(updated.level, 0);
  EXPECT_EQ(updated.speed, 1);
  EXPECT_EQ(updated.pause, 0);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, UpdateCurrentState) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  GameInfo_t state1 = updateCurrentState();
  EXPECT_EQ(state1.score, 0);
  EXPECT_EQ(state1.pause, 0);
  ASSERT_NE(state1.field, nullptr);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, UserInputLeftRight) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  GameInfo_t state_before = updateCurrentState();
  int min_x_before = FIELD_W, min_x_after = FIELD_W;

  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      if (state_before.field[i][j] == 1) {
        if (j < min_x_before) min_x_before = j;
      }
    }
  }

  userInput(Right, false);
  GameInfo_t state_after = updateCurrentState();
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      if (state_after.field[i][j] == 1) {
        if (j < min_x_after) min_x_after = j;
      }
    }
  }
  EXPECT_EQ(min_x_after, min_x_before + 1);

  userInput(Left, false);
  state_after = updateCurrentState();
  min_x_after = FIELD_W;
  for (int i = 0; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      if (state_after.field[i][j] == 1) {
        if (j < min_x_after) min_x_after = j;
      }
    }
  }
  EXPECT_EQ(min_x_after, min_x_before);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, UserInputPause) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  GameInfo_t state = updateCurrentState();
  EXPECT_EQ(state.pause, 0);

  userInput(Pause, false);
  state = updateCurrentState();
  EXPECT_EQ(state.pause, 1);

  userInput(Pause, false);
  state = updateCurrentState();
  EXPECT_EQ(state.pause, 0);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, GenerateRandomTetromino) {
  Tetromino t = generate_random_tetromino();
  EXPECT_TRUE(t.size == 2 || t.size == 3 || t.size == 4);

  int sum = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      sum += t.shape[i][j];
    }
  }
  EXPECT_GT(sum, 0);
}

TEST(TetrisBackendTest, ClearLines) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  for (int j = 0; j < FIELD_W; j++) {
    state->game_info.field[FIELD_H - 1][j] = 2;
  }

  int old_score = state->game_info.score;
  clear_lines(state, state->game_info.field);

  for (int j = 0; j < FIELD_W; j++) {
    EXPECT_EQ(state->game_info.field[FIELD_H - 1][j], 0);
  }

  EXPECT_GT(state->game_info.score, old_score);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, NextLevel) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  state->game_info.score = 1200;
  GameInfo_t updated = updateCurrentState();
  EXPECT_EQ(updated.level, 2);
  EXPECT_EQ(updated.speed, 2);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, GameOver) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      state->game_info.field[i][j] = 2;
    }
  }

  state->current.y = 0;

  bool collision = check_collision(state, 0, 0);
  EXPECT_TRUE(collision);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, TetrominoRotation) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  int original_shape[4][4];
  memcpy(original_shape, state->current.shape, sizeof(original_shape));

  userInput(Action, false);
  GameInfo_t updated = updateCurrentState();

  bool changed = false;
  for (int i = 0; i < 4 && !changed; ++i) {
    for (int j = 0; j < 4 && !changed; ++j) {
      if (original_shape[i][j] != state->current.shape[i][j]) {
        changed = true;
      }
    }
  }
  EXPECT_TRUE(changed);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, ClearLinesOne) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  for (int j = 0; j < FIELD_W; j++) {
    state->game_info.field[FIELD_H - 1][j] = 2;
  }

  int old_score = state->game_info.score;
  clear_lines(state, state->game_info.field);
  EXPECT_GT(state->game_info.score, old_score);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, ClearLinesTwo) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  for (int i = FIELD_H - 2; i < FIELD_H; i++) {
    for (int j = 0; j < FIELD_W; j++) {
      state->game_info.field[i][j] = 2;
    }
  }

  int old_score = state->game_info.score;
  clear_lines(state, state->game_info.field);
  EXPECT_GT(state->game_info.score, old_score);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, LockTetromino) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  Tetromino o = {.shape = {{1, 1}, {1, 1}}, .size = 2, .x = 3, .y = 5};
  state->current = o;
  lock_tetromino(state, state->game_info.field);

  for (int di = 0; di < 2; di++) {
    for (int dj = 0; dj < 2; dj++) {
      int yy = o.y + di, xx = o.x + dj;
      EXPECT_EQ(state->game_info.field[yy][xx], 2);
    }
  }

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, LevelUp) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  state->game_info.score = 1200;
  GameInfo_t st = updateCurrentState();
  EXPECT_EQ(st.level, 2);
  EXPECT_EQ(st.speed, 2);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

TEST(TetrisBackendTest, PauseToggle) {
  GameInfo_t game_info;
  init_game_info(&game_info);
  TetrisState* state = S();

  EXPECT_EQ(state->game_info.pause, 0);
  userInput(Pause, false);
  GameInfo_t updated = updateCurrentState();
  EXPECT_EQ(updated.pause, 1);
  userInput(Pause, false);
  updated = updateCurrentState();
  EXPECT_EQ(updated.pause, 0);

  free_matrix(FIELD_H, game_info.field);
  free_matrix(4, game_info.next);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
