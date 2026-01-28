#include <gtest/gtest.h>

#include "brick_game/common/common_backend.h"
#include "brick_game/snake/backend.h"
#include "brick_game/snake/snake_game.h"

using namespace s21;

TEST(SnakeGameTest, HeadDoesNotMoveOnFirstTick) {
  SnakeGame game;
  game.init();
  auto old_head = game.getHead();
  game.tick();
  auto new_head = game.getHead();
  EXPECT_EQ(new_head.x, old_head.x);
  EXPECT_EQ(new_head.y, old_head.y);
}

TEST(SnakeGameTest, AccelerationIncreasesSpeed) {
  SnakeGame game;
  game.init();
  game.tick();
  int base_speed = game.tick().speed;
  game.handleInput(UserAction_t::Action, true);
  int fast_speed = game.tick().speed;
  EXPECT_GT(base_speed, 0);
  EXPECT_GT(fast_speed, base_speed);
}

TEST(SnakeGameTest, EatAppleIncreasesScore) {
  SnakeGame game;
  game.init();
  game.tick();
  int old_score = game.tick().score;
  auto head = game.getHead();
  game.setAppleForTest(head.x + 1, head.y);
  game.handleInput(UserAction_t::Right, false);
  for (int i = 0; i < 12; ++i) game.tick();
  int new_score = game.tick().score;
  EXPECT_EQ(new_score, old_score + 1);
}

TEST(SnakeGameTest, GameOverOnWallCollision) {
  SnakeGame game;
  game.init();
  game.tick();
  game.handleInput(UserAction_t::Left, false);
  for (int i = 0; i < FIELD_W; ++i) {
    for (int j = 0; j < 12; ++j) game.tick();
  }
  GameInfo_t info = game.tick();
  EXPECT_EQ(info.pause, 111);
}

TEST(SnakeGameTest, HighScorePersists) {
  GameInfo_t info;
  snake_init_game_info(&info);
  int old_high = info.high_score;
  info.score = old_high + 10;
  save_high_score("snake", info.score);
  int loaded = load_high_score("snake");
  EXPECT_EQ(loaded, info.score);
}

TEST(SnakeGameTest, LevelIncreasesEvery5Points) {
  SnakeGame game;
  game.init();
  int last_level = 1;
  int score = 0;
  for (int i = 0; i < 30; ++i) {
    int old_score = game.tick().score;
    int new_score = old_score;
    for (int step = 0; step < 200 && new_score == old_score; ++step) {
      game.handleInput(UserAction_t::Right, false);
      for (int j = 0; j < 12; ++j) game.tick();
      new_score = game.tick().score;
    }
    score = new_score;
    int level = game.tick().level;
    if (score % 5 == 0 && score != 0) EXPECT_EQ(level, last_level + 1);
    last_level = level;
  }
}

TEST(SnakeGameTest, PauseFunctionality) {
  SnakeGame game;
  game.init();
  game.tick();

  game.handleInput(UserAction_t::Pause, false);
  GameInfo_t info = game.tick();
  EXPECT_EQ(info.pause, 333);

  Point head_before = game.getHead();
  for (int i = 0; i < 50; ++i) game.tick();
  Point head_after = game.getHead();
  EXPECT_EQ(head_before.x, head_after.x);
  EXPECT_EQ(head_before.y, head_after.y);

  game.handleInput(UserAction_t::Pause, false);
  info = game.tick();
  EXPECT_EQ(info.pause, 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}