#pragma once
#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class TetrisGameWidget;

class TetrisWindow : public QMainWindow {
  Q_OBJECT
 public:
  TetrisWindow(QWidget *parent = nullptr);
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
  TetrisGameWidget *gameWidget;
  QWidget *nextWidget;
};