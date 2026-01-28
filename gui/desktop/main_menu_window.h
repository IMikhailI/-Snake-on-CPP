#pragma once
#include <QMainWindow>
#include <QPushButton>

class MainMenuWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainMenuWindow(QWidget *parent = nullptr);
 private slots:
  void startSnake();
  void startTetris();
  void exitApp();
};