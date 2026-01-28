#pragma once
#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class GameWidget;

class SnakeWindow : public QMainWindow {
  Q_OBJECT
 public:
  SnakeWindow(QWidget *parent = nullptr);
  void closeEvent(QCloseEvent *event) override;
 signals:
  void aboutToClose();
 private slots:
  void updateSidebar();
  void restartGame();

 private:
  QLabel *sidebarLabel;
  QPushButton *restartButton;
  QPushButton *exitButton;
  GameWidget *gameWidget;
};