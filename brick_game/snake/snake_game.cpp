#include "snake_game.h"

#include <algorithm>
#include <cstring>

#include "../common/common_backend.h"

using namespace s21;

SnakeGame::SnakeGame()
    : snake_(),
      apple_(),
      dir_(Direction::Right),
      score_(0),
      high_score_(load_high_score("snake")),
      level_(1),
      base_speed_(3),
      accel_speed_(7),
      speed_(base_speed_),
      frame_count_(0),
      rng_(std::random_device{}()),
      state_(State::Start) {
  field_ = create_matrix(FIELD_H, FIELD_W);
}

SnakeGame::~SnakeGame() { free_matrix(FIELD_H, field_); }

void SnakeGame::init() {
  snake_.clear();
  int midx = FIELD_W / 2;
  for (int i = 0; i < INIT_LENGTH; ++i) {
    snake_.push_back({midx - i, FIELD_H / 2});
  }
  dir_ = Direction::Right;
  score_ = 0;
  level_ = 1;
  speed_ = base_speed_;
  frame_count_ = 0;
  state_ = State::Start;
  placeApple();
  for (int y = 0; y < FIELD_H; ++y)
    std::memset(field_[y], 0, FIELD_W * sizeof(int));
}

void SnakeGame::handleInput(UserAction_t action, bool hold) {
  if (state_ == State::GameOver || state_ == State::Win) {
    if (action == Action && hold) init();
    return;
  }

  if (action == Pause && !hold) {
    if (state_ == State::Moving || state_ == State::Start) {
      state_ = State::Pause;
    } else if (state_ == State::Pause) {
      state_ = State::Moving;
    }
    return;
  }

  if (action == Action) {
    if (state_ == State::Moving || state_ == State::Start)
      speed_ = hold ? accel_speed_ : base_speed_;
    return;
  }

  if (state_ == State::Moving || state_ == State::Start) {
    Direction nd = dir_;
    switch (action) {
      case Left:
        nd = Direction::Left;
        break;
      case Right:
        nd = Direction::Right;
        break;
      case Up:
        nd = Direction::Up;
        break;
      case Down:
        nd = Direction::Down;
        break;
      default:
        return;
    }
    if (!((dir_ == Direction::Left && nd == Direction::Right) ||
          (dir_ == Direction::Right && nd == Direction::Left) ||
          (dir_ == Direction::Up && nd == Direction::Down) ||
          (dir_ == Direction::Down && nd == Direction::Up)))
      dir_ = nd;
  }
}

GameInfo_t SnakeGame::tick() {
  GameInfo_t info;
  for (int y = 0; y < FIELD_H; ++y)
    std::memset(field_[y], 0, FIELD_W * sizeof(int));
  info.field = field_;
  info.next = nullptr;

  if (state_ == State::Start) {
    state_ = State::Moving;
  }

  if (state_ == State::Moving) {
    frame_count_++;
    constexpr int baseThreshold = 10;
    constexpr int multiplier = 2;
    int threshold =
        std::max(4, (baseThreshold - speed_ - (level_ - 1)) * multiplier);
    if (frame_count_ >= threshold) {
      frame_count_ = 0;

      Point head = snake_.front();
      Point next = head;
      switch (dir_) {
        case Direction::Left:
          --next.x;
          break;
        case Direction::Right:
          ++next.x;
          break;
        case Direction::Up:
          --next.y;
          break;
        case Direction::Down:
          ++next.y;
          break;
      }
      if (collision(next)) {
        state_ = State::GameOver;
      } else {
        snake_.push_front(next);
        if (next == apple_) {
          ++score_;
          state_ = State::Grow;
        } else {
          snake_.pop_back();
        }
      }
    }
  }
  if (state_ == State::Grow) {
    placeApple();
    if ((int)snake_.size() >= WIN_LENGTH) {
      state_ = State::Win;
    } else {
      int new_level = std::min(10, score_ / 5 + 1);
      if (new_level != level_) {
        level_ = new_level;
        base_speed_ = level_;
      }
      state_ = State::Moving;
    }
  }

  for (auto &p : snake_) {
    field_[p.y][p.x] = 1;
  }
  field_[apple_.y][apple_.x] = 2;

  info.score = score_;
  if (score_ > high_score_) {
    high_score_ = score_;
    save_high_score("snake", high_score_);
  }
  info.high_score = high_score_;
  info.level = level_;
  info.speed = speed_;
  // pause: 111 = GameOver, 222 = Win, 333 = Pause, 0 = running
  info.pause = (state_ == State::GameOver) ? 111
               : (state_ == State::Win)    ? 222
               : (state_ == State::Pause)  ? 333
                                           : 0;

  return info;
}

void SnakeGame::placeApple() {
  std::uniform_int_distribution<int> dx(0, FIELD_W - 1), dy(0, FIELD_H - 1);
  Point p;
  do {
    p.x = dx(rng_);
    p.y = dy(rng_);
  } while (collision(p));
  apple_ = p;
}

bool SnakeGame::collision(const Point &p) const {
  if (p.x < 0 || p.x >= FIELD_W || p.y < 0 || p.y >= FIELD_H) return true;
  return std::find(snake_.begin(), snake_.end(), p) != snake_.end();
}