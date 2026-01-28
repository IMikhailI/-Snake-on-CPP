#include "tetris_game_widget.h"

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>

const int CELL_SIZE = 30;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 20;

TetrisGameWidget::TetrisGameWidget(QWidget *parent) : QWidget(parent) {
  setFocusPolicy(Qt::StrongFocus);
  setMinimumSize(FIELD_WIDTH * CELL_SIZE + 2, FIELD_HEIGHT * CELL_SIZE + 2);
  startGame();
  connect(&timer, &QTimer::timeout, this, &TetrisGameWidget::gameTick);
  timer.start(120);
}

void TetrisGameWidget::startGame() { init_game_info(&gameInfo); }

void TetrisGameWidget::gameTick() {
  gameInfo = updateCurrentState();
  update();
}

void TetrisGameWidget::handleUserInput(int key, bool hold) {
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
  userInput(action, hold);
}

void TetrisGameWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Space:
    case Qt::Key_Up:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Down:
    case Qt::Key_P:
      handleUserInput(event->key(), false);
      break;
    default:
      break;
  }
  QWidget::keyPressEvent(event);
}

void TetrisGameWidget::keyReleaseEvent(QKeyEvent *event) {
  QWidget::keyReleaseEvent(event);
}

void TetrisGameWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing, false);
  painter.fillRect(rect(), QColor(30, 30, 40));
  painter.setPen(QPen(QColor(180, 180, 180), 2));
  painter.drawRect(0, 0, FIELD_WIDTH * CELL_SIZE, FIELD_HEIGHT * CELL_SIZE);
  painter.setPen(QPen(QColor(60, 60, 80), 1));
  for (int x = 1; x < FIELD_WIDTH; ++x)
    painter.drawLine(x * CELL_SIZE, 0, x * CELL_SIZE, FIELD_HEIGHT * CELL_SIZE);
  for (int y = 1; y < FIELD_HEIGHT; ++y)
    painter.drawLine(0, y * CELL_SIZE, FIELD_WIDTH * CELL_SIZE, y * CELL_SIZE);
  if (!gameInfo.field) return;
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      if (gameInfo.field[y][x] == 2) {
        painter.setBrush(QColor(80, 180, 220));
        painter.setPen(Qt::NoPen);
        painter.drawRect(x * CELL_SIZE + 2, y * CELL_SIZE + 2, CELL_SIZE - 4,
                         CELL_SIZE - 4);
      } else if (gameInfo.field[y][x] == 1) {
        painter.setBrush(QColor(220, 220, 80));
        painter.setPen(Qt::NoPen);
        painter.drawRect(x * CELL_SIZE + 2, y * CELL_SIZE + 2, CELL_SIZE - 4,
                         CELL_SIZE - 4);
      }
    }
  }
  if (gameInfo.pause == 111) {
    painter.setPen(QColor(255, 60, 60));
    QFont font = painter.font();
    font.setPointSize(22);
    painter.setFont(font);
    painter.drawText(width() / 2 - 70, height() / 2, "Game Over");
  }
}