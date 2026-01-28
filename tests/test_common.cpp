#include <gtest/gtest.h>

#include <cstdio>
#include <cstring>

#include "brick_game/common/common_backend.h"

TEST(CommonBackendTest, SaveAndLoadHighScore) {
  const char* game = "testgame";
  int score = 12345;
  save_high_score(game, score);
  int loaded = load_high_score(game);
  EXPECT_EQ(loaded, score);
  char filename[64];
  snprintf(filename, sizeof filename, "%s_highscore.txt", game);
  std::remove(filename);
}

TEST(CommonBackendTest, LoadHighScore_FileNotExist) {
  const char* game = "nonexistentgame";
  char filename[64];
  snprintf(filename, sizeof filename, "%s_highscore.txt", game);
  std::remove(filename);
  int loaded = load_high_score(game);
  EXPECT_EQ(loaded, 0);
}

TEST(CommonBackendTest, LoadHighScore_FileCorrupted) {
  const char* game = "corruptedgame";
  char filename[64];
  snprintf(filename, sizeof filename, "%s_highscore.txt", game);
  FILE* f = fopen(filename, "w");
  fprintf(f, "notanumber");
  fclose(f);
  int loaded = load_high_score(game);
  EXPECT_EQ(loaded, 0);
  std::remove(filename);
}

TEST(CommonBackendTest, CreateAndFreeMatrix) {
  int N = 5, M = 7;
  int** matrix = create_matrix(N, M);
  ASSERT_NE(matrix, nullptr);
  for (int i = 0; i < N; ++i) {
    ASSERT_NE(matrix[i], nullptr);
    for (int j = 0; j < M; ++j) {
      EXPECT_EQ(matrix[i][j], 0);
    }
  }
  free_matrix(N, matrix);
}

TEST(CommonBackendTest, FreeMatrixNull) { free_matrix(10, nullptr); }