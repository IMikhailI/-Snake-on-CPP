#include "tetris_window.h"

#include <QCloseEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "tetris_game_widget.h"
extern "C" {
#include "tetris_backend.h"
}

class NextFigureWidget : public QWidget {
 public:
  NextFigureWidget(QWidget *parent = nullptr) : QWidget(parent) {
    setFixedSize(80, 80);
  }

 protected:
  void paintEvent(QPaintEvent *) override {
    TetrisState *state = get_state();
    if (!state) return;
    const Tetromino &next = state->next;
    QPainter p(this);
    p.fillRect(rect(), QColor(34, 34, 44));
    int cell = 16;
    int offsetX = (width() - cell * next.size) / 2;
    int offsetY = (height() - cell * next.size) / 2;
    for (int i = 0; i < next.size; ++i) {
      for (int j = 0; j < next.size; ++j) {
        if (next.shape[i][j]) {
          p.setBrush(QColor(220, 220, 80));
          p.setPen(Qt::NoPen);
          p.drawRect(offsetX + j * cell, offsetY + i * cell, cell - 2,
                     cell - 2);
        }
      }
    }
  }
};

TetrisWindow::TetrisWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *central = new QWidget(this);
  QHBoxLayout *mainLayout = new QHBoxLayout(central);
  gameWidget = new TetrisGameWidget(central);
  QVBoxLayout *sidebarLayout = new QVBoxLayout();
  sidebarLabel = new QLabel(central);
  sidebarLabel->setMinimumWidth(140);
  sidebarLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  sidebarLabel->setStyleSheet(
      "color: white; font-size: 16px; background: #222; padding: 10px;");
  sidebarLayout->addWidget(sidebarLabel);
  QLabel *nextLabel = new QLabel("Next:", central);
  nextLabel->setStyleSheet("color: #aaa; font-size: 14px; margin-top: 10px;");
  sidebarLayout->addWidget(nextLabel);
  nextWidget = new NextFigureWidget(central);
  sidebarLayout->addWidget(nextWidget);
  restartButton = new QPushButton("Restart", central);
  exitButton = new QPushButton("Exit", central);
  sidebarLayout->addSpacing(20);
  sidebarLayout->addWidget(restartButton);
  sidebarLayout->addWidget(exitButton);
  sidebarLayout->addStretch();
  mainLayout->addWidget(gameWidget, 1);
  mainLayout->addLayout(sidebarLayout);
  setCentralWidget(central);
  setWindowTitle("Tetris Game (Qt)");
  QTimer *sidebarTimer = new QTimer(this);
  connect(sidebarTimer, &QTimer::timeout, this, &TetrisWindow::updateSidebar);
  connect(sidebarTimer, &QTimer::timeout, nextWidget,
          QOverload<>::of(&QWidget::update));
  sidebarTimer->start(100);
  updateSidebar();
  connect(restartButton, &QPushButton::clicked, this,
          &TetrisWindow::restartGame);
  connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
}

void TetrisWindow::updateSidebar() {
  const GameInfo_t &info = gameWidget->getGameInfo();
  QString text = QString(
                     "<b>Score:</b> %1<br>"
                     "<b>Level:</b> %2<br>"
                     "<b>High Score:</b> %3<br>")
                     .arg(info.score)
                     .arg(info.level)
                     .arg(info.high_score);
  if (info.pause == 111) text += "<br><span style='color:red'>Game Over</span>";
  text +=
      "<hr><b>Управление:</b><br>"
      "← → — перемещение фигуры<br>"
      "↓ — ускорить падение<br>"
      "Space — сброс<br>"
      "P — пауза<br>"
      "Restart — начать заново<br>"
      "Exit — выход";
  sidebarLabel->setText(text);
}

void TetrisWindow::restartGame() { gameWidget->startGame(); }

void TetrisWindow::closeEvent(QCloseEvent *event) {
  emit aboutToClose();
  QMainWindow::closeEvent(event);
}