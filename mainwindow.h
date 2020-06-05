#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMouseEvent>
#include <QWidget>
#include <QPainter>
#include "board.h"
#include "game.h"
#include <math.h>
class MainWindow : public QWidget
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();
  const int SIZE=10;
  const int WIDTH=40;
  const int x=20,y=20;
private slots:

private:
  Game *game;
  void paintEvent(QPaintEvent *);//绘图函数声明
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void chessOneByPerson(); //由人下棋
  int clickPosRow, clickPosCol; //存储点击位置
  QPainter *paint;
};

#endif // MAINWINDOW_H
