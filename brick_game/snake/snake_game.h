#pragma once

#include <deque>
#include <random>

#include "../common/common_backend.h"

namespace s21 {

enum class State { Start, Moving, Grow, GameOver, Win, Pause };

struct Point {
  int x, y;
  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
};
enum class Direction { Left, Right, Up, Down };

class SnakeGame {
 public:
  SnakeGame();
  ~SnakeGame();

  void init();
  void handleInput(UserAction_t action, bool hold);
  GameInfo_t tick();

  // Для тестов: получить координаты головы змейки
  Point getHead() const { return snake_.front(); }
  // Для тестов: установить яблоко в заданные координаты
  void setAppleForTest(int x, int y) { apple_ = {x, y}; }

 private:
  static constexpr int INIT_LENGTH = 4;
  static constexpr int WIN_LENGTH = 200;

  std::deque<Point> snake_;
  Point apple_;
  Direction dir_;
  int score_;
  int high_score_;
  int level_;
  int base_speed_;
  int accel_speed_;
  int speed_;
  int frame_count_;
  int **field_ = nullptr;
  std::mt19937_64 rng_;
  bool paused_ = false;
  State state_;

  void placeApple();
  bool collision(const Point &p) const;
};

}  // namespace s21
