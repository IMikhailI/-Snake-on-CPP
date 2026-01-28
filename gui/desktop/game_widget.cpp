#include "game_widget.h"

#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

const int CELL_SIZE = 30;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 20;

GameWidget::GameWidget(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  setMinimumSize(FIELD_WIDTH * CELL_SIZE + 2, FIELD_HEIGHT * CELL_SIZE + 2);
  startGame();
  connect(&timer, &QTimer::timeout, this, &GameWidget::gameTick);
  timer.start(120);
}

void GameWidget::startGame() { snake_init_game_info(&gameInfo); }

void GameWidget::gameTick() {
  gameInfo = snake_updateCurrentState();
  update();
}

void GameWidget::handleUserInput(int key, bool hold) {
  UserAction_t action = Action;
  switch (key) {
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Space:
      action = Action;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    default:
      return;
  }
  snake_userInput(action, hold);
}

void GameWidget::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    handleUserInput(Qt::Key_Space, true);
  } else {
    handleUserInput(event->key(), false);
  }
  QWidget::keyPressEvent(event);
}

void GameWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Space) {
    handleUserInput(Qt::Key_Space, false);
  }
  QWidget::keyReleaseEvent(event);
}

void GameWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);
  // Фон
  painter.fillRect(rect(), QColor(30, 30, 40));
  // Рамка
  painter.setPen(QPen(QColor(180, 180, 180), 2));
  painter.drawRect(0, 0, FIELD_WIDTH * CELL_SIZE, FIELD_HEIGHT * CELL_SIZE);
  // Сетка
  painter.setPen(QPen(QColor(60, 60, 80), 1));
  for (int x = 1; x < FIELD_WIDTH; ++x)
    painter.drawLine(x * CELL_SIZE, 0, x * CELL_SIZE, FIELD_HEIGHT * CELL_SIZE);
  for (int y = 1; y < FIELD_HEIGHT; ++y)
    painter.drawLine(0, y * CELL_SIZE, FIELD_WIDTH * CELL_SIZE, y * CELL_SIZE);
  if (!gameInfo.field) return;
  // Яблоко
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      if (gameInfo.field[y][x] == 2) {
        painter.setBrush(QColor(220, 50, 50));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(x * CELL_SIZE + 4, y * CELL_SIZE + 4, CELL_SIZE - 8,
                            CELL_SIZE - 8);
      }
    }
  }
  // Змейка
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      if (gameInfo.field[y][x] == 1) {
        painter.setBrush(QColor(40, 200, 80));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(x * CELL_SIZE + 2, y * CELL_SIZE + 2,
                                CELL_SIZE - 4, CELL_SIZE - 4, 6, 6);
      }
    }
  }
  // Сообщение о проигрыше/победе
  if (gameInfo.pause == 111) {
    painter.setPen(QColor(255, 60, 60));
    QFont font = painter.font();
    font.setPointSize(22);
    painter.setFont(font);
    painter.drawText(width() / 2 - 70, height() / 2, "Game Over");
  } else if (gameInfo.pause == 222) {
    painter.setPen(QColor(255, 220, 60));
    QFont font = painter.font();
    font.setPointSize(22);
    painter.setFont(font);
    painter.drawText(width() / 2 - 70, height() / 2, "You Win!");
  } else if (gameInfo.pause == 333) {
    painter.setPen(QColor(60, 150, 255));
    QFont font = painter.font();
    font.setPointSize(22);
    painter.setFont(font);
    painter.drawText(width() / 2 - 50, height() / 2, "PAUSED");
  }
}