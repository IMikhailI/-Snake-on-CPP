#include "main_menu_window.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "snake_window.h"
#include "tetris_window.h"

MainMenuWindow::MainMenuWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *central = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(central);
  QLabel *title = new QLabel("<h2>BrickGame v2.0</h2>", central);
  title->setAlignment(Qt::AlignCenter);
  layout->addWidget(title);
  QPushButton *snakeBtn = new QPushButton("Snake", central);
  QPushButton *tetrisBtn = new QPushButton("Tetris", central);
  QPushButton *exitBtn = new QPushButton("Exit", central);
  layout->addWidget(snakeBtn);
  layout->addWidget(tetrisBtn);
  layout->addWidget(exitBtn);
  layout->addStretch();
  setCentralWidget(central);
  setWindowTitle("BrickGame v2.0 — Main Menu");
  resize(300, 200);
  connect(snakeBtn, &QPushButton::clicked, this, &MainMenuWindow::startSnake);
  connect(tetrisBtn, &QPushButton::clicked, this, &MainMenuWindow::startTetris);
  connect(exitBtn, &QPushButton::clicked, this, &MainMenuWindow::exitApp);
}

void MainMenuWindow::startSnake() {
  auto *w = new SnakeWindow();
  connect(w, &SnakeWindow::aboutToClose, this, [this]() { this->show(); });
  w->show();
  close();
}
void MainMenuWindow::startTetris() {
  auto *w = new TetrisWindow();
  connect(w, &TetrisWindow::aboutToClose, this, [this]() { this->show(); });
  w->show();
  close();
}
void MainMenuWindow::exitApp() { close(); }