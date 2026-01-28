#include "snake_window.h"

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "game_widget.h"

SnakeWindow::SnakeWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *central = new QWidget(this);
  QHBoxLayout *mainLayout = new QHBoxLayout(central);
  gameWidget = new GameWidget(central);
  QVBoxLayout *sidebarLayout = new QVBoxLayout();
  sidebarLabel = new QLabel(central);
  sidebarLabel->setMinimumWidth(140);
  sidebarLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  sidebarLabel->setStyleSheet(
      "color: white; font-size: 16px; background: #222; padding: 10px;");
  sidebarLayout->addWidget(sidebarLabel);
  restartButton = new QPushButton("Restart", central);
  exitButton = new QPushButton("Exit", central);
  sidebarLayout->addSpacing(20);
  sidebarLayout->addWidget(restartButton);
  sidebarLayout->addWidget(exitButton);
  sidebarLayout->addStretch();
  mainLayout->addWidget(gameWidget, 1);
  mainLayout->addLayout(sidebarLayout);
  setCentralWidget(central);
  setWindowTitle("Snake Game (Qt)");
  QTimer *sidebarTimer = new QTimer(this);
  connect(sidebarTimer, &QTimer::timeout, this, &SnakeWindow::updateSidebar);
  sidebarTimer->start(100);
  updateSidebar();
  connect(restartButton, &QPushButton::clicked, this,
          &SnakeWindow::restartGame);
  connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
}

void SnakeWindow::updateSidebar() {
  const GameInfo_t &info = gameWidget->getGameInfo();
  QString text = QString(
                     "<b>Score:</b> %1<br>"
                     "<b>Level:</b> %2<br>"
                     "<b>High Score:</b> %3<br>")
                     .arg(info.score)
                     .arg(info.level)
                     .arg(info.high_score);
  if (info.pause == 111)
    text += "<br><span style='color:red'>Game Over</span>";
  else if (info.pause == 222)
    text += "<br><span style='color:yellow'>You Win!</span>";
  text +=
      "<hr><b>Управление:</b><br>"
      "← → — поворот змейки<br>"
      "Space — ускорение<br>"
      "Restart — начать заново<br>"
      "Exit — выход";
  sidebarLabel->setText(text);
}

void SnakeWindow::restartGame() { gameWidget->startGame(); }

void SnakeWindow::closeEvent(QCloseEvent *event) {
  emit aboutToClose();
  QMainWindow::closeEvent(event);
}