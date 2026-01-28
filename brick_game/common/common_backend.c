#include "common_backend.h"

#include <stdio.h>
#include <stdlib.h>

int load_high_score(const char *game_name) {
  char filename[64];
  snprintf(filename, sizeof filename, "%s_highscore.txt", game_name);
  FILE *f = fopen(filename, "r");
  int high = 0;
  if (f) {
    if (fscanf(f, "%d", &high) != 1) high = 0;
    fclose(f);
  }
  return high;
}

void save_high_score(const char *game_name, int high_score) {
  char filename[64];
  snprintf(filename, sizeof filename, "%s_highscore.txt", game_name);
  FILE *f = fopen(filename, "w");
  if (f) {
    fprintf(f, "%d", high_score);
    fclose(f);
  }
}

int **create_matrix(int N, int M) {
  int **result = calloc(N, sizeof(int *));
  bool error_occurred = false;

  if (result != NULL) {
    for (int i = 0; i < N; i++) {
      if (!error_occurred) {
        result[i] = calloc(M, sizeof(int));
        if (result[i] == NULL) {
          error_occurred = true;
        }
      } else {
        result[i] = NULL;
      }
    }

    if (error_occurred) {
      for (int i = 0; i < N; i++)
        if (result[i] != NULL) free(result[i]);
      free(result);
      return NULL;
    }
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++) result[i][j] = 0;
  }
  return result;
}

void free_matrix(int N, int **matrix) {
  if (matrix != NULL) {
    for (int i = 0; i < N; i++) free(matrix[i]);
    free(matrix);
  }
}
