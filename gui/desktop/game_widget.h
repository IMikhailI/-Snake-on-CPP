#pragma once
#include <QTimer>
#include <QWidget>
extern "C" {
#include "backend.h"
}

class GameWidget : public QWidget {
  Q_OBJECT
 public:
  explicit GameWidget(QWidget *parent = nullptr);
  const GameInfo_t &getGameInfo() const { return gameInfo; }
  void startGame();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
 private slots:
  void gameTick();

 private:
  GameInfo_t gameInfo;
  QTimer timer;
  void handleUserInput(int key, bool hold);
};