
#include<QDebug>
#include<QMessageBox>
#include <utility>
#include <QMediaPlayer>
#include <thread>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QPainter>
#include <QTimer>
#include <QSound>
#include <unistd.h>
#include <QMainWindow>
#include <qmainwindow.h>
#include "mainwindow.h"

const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kBlockSize = 40; // 格子的大小
const int kMarkSize = 6; // 落子标记边长
const int kPosDelta = 20; // 鼠标点击的模糊距离上限
int over = 0;
int end = 0;
int Player1type = 1, Player2type = 1;
extern int lastx, lasty;
int Depth = 7; //搜索树深度

void MainWindow::setPlayer1Bot() {
    Player1type = 0;
    game->setPlayer(0, Player1type);
}
void MainWindow::setPlayer1Human() {
    Player1type = 1;
    game->setPlayer(0, Player1type);
}
void MainWindow::setPlayer2Bot() {
    Player2type = 0;
    game->setPlayer(1, Player2type);
}
void MainWindow::setPlayer2Human() {
    Player2type = 1;
    game->setPlayer(1, Player2type);
}
void MainWindow::setDeep5() {
    Depth = 5;
}

void MainWindow::setDeep7() {
    Depth = 7;
}

void MainWindow::setDeep9() {
    Depth = 9;
}

void MainWindow::withdraw() {
    int x, y;
    x = game->history.back().first;
    y = game->history.back().second;
    game->history.pop_back();
    game->setStatus(x, y, 0);
    x = game->history.back().first;
    y = game->history.back().second;
    game->history.pop_back();
    game->setStatus(x, y, 0);
    repaint();
}
MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent) {

    setFixedSize(kBoardMargin * 2 + kBlockSize * (kBoardSizeNum - 1), kBoardMargin * 2 + kBlockSize * (kBoardSizeNum - 1));
    setMouseTracking(true);
    QMenu *gameMenu = menuBar()->addMenu(tr("Game"));
    QMenu *Player1Type = new QMenu("Player1 type", this);
    gameMenu->addMenu(Player1Type);
    QMenu *Player2Type = new QMenu("Player2 type", this);
    QMenu *Deep = new QMenu("Deep", this);
    gameMenu->addMenu(Player2Type);
    gameMenu->addMenu(Deep);
    QAction *setPlayer1Bot = new QAction("Bot", this);
    QAction *setPlayer1Human = new QAction("Human", this);
    connect(setPlayer1Bot, SIGNAL(triggered()), this, SLOT(setPlayer1Bot()));
    Player1Type->addAction(setPlayer1Bot);
    connect(setPlayer1Human, SIGNAL(triggered()), this, SLOT(setPlayer1Human()));
    Player1Type->addAction(setPlayer1Human);
    QAction *setPlayer2Bot = new QAction("Bot", this);
    QAction *setPlayer2Human = new QAction("Human", this);
    connect(setPlayer2Bot, SIGNAL(triggered()), this, SLOT(setPlayer2Bot()));
    Player2Type->addAction(setPlayer2Bot);
    connect(setPlayer2Human, SIGNAL(triggered()), this, SLOT(setPlayer2Human()));
    Player2Type->addAction(setPlayer2Human);
    QAction *withdraw = new QAction("withdraw", this);
    connect(withdraw, SIGNAL(triggered()), this, SLOT(withdraw()));
    gameMenu->addAction(withdraw);
    QAction *setDeep5 = new QAction("5", this);
    connect(setDeep5, SIGNAL(triggered()), this, SLOT(setDeep5()));
    Deep->addAction(setDeep5);
    QAction *setDeep7 = new QAction("7", this);
    connect(setDeep7, SIGNAL(triggered()), this, SLOT(setDeep7()));
    Deep->addAction(setDeep7);
    QAction *setDeep9 = new QAction("9", this);
    connect(setDeep9, SIGNAL(triggered()), this, SLOT(setDeep9()));
    Deep->addAction(setDeep9);
    Board *board = new Board;
    Player *player1 = new Player(Player1type, 1, board);
    Player *player2 = new Player(Player2type, -1, board);
    game = new Game(board, player1, player2);

}

MainWindow::~MainWindow() {
    delete game;
}
int pp = 0;

void MainWindow::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  // 绘制棋盘
  painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
  for (int i = 0; i < kBoardSizeNum; ++i) {
      painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
      painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
  }
  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  //绘制落点预测
  if (clickPosRow >= 0 && clickPosRow < kBoardSizeNum &&
      clickPosCol >= 0 && clickPosCol < kBoardSizeNum &&
      game->getStatus(clickPosRow, clickPosCol) == 0) {
      if (game->getPlayer()->getColor() == -1)
          brush.setColor(Qt::white);
      else
          brush.setColor(Qt::black);
      painter.setBrush(brush);
      painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
  }

  //绘制棋子
  for(int i = 0; i < kBoardSizeNum; ++i) {
      for (int j = 0; j < kBoardSizeNum; ++j) {
          if (game->getStatus(i, j) == 1)           //说明这个位置是黑棋
          {
              brush.setColor(Qt::black);
              painter.setBrush(brush);
              painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
          }
          else if (game->getStatus(i, j) == -1)     //说明这个位置是白棋
          {
              brush.setColor(Qt::white);
              painter.setBrush(brush);
              painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius, kBoardMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
          }
          if(lastx == i && lasty == j) {
                brush.setColor(Qt::red);
                painter.setBrush(brush);
                painter.drawEllipse(kBoardMargin + kBlockSize * j - kRadius / 5, kBoardMargin + kBlockSize * i - kRadius / 5, kRadius * 2 / 5, kRadius * 2 / 5);
          }
      }
  }
  if(end) {
      QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", " win!");

      delete game;
      Board *board = new Board;
      Player *player1 = new Player(Player1type, 1, board);
      Player *player2 = new Player(Player2type, -1, board);
      game = new Game(board, player1, player2);
      setMouseTracking(true);
      end = 0;
      update();
  }
  if(over) {
    setMouseTracking(false);
    over = 0;
    end = 1;
    qDebug() << "win";

    update();
  }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 获取鼠标当前的坐标
    int x = event->x();
    int y = event->y();

    if (x >= kBoardMargin - kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y >= kBoardMargin - kBlockSize / 2 &&
            y < size().height()- kBoardMargin) {
        // 获取最近的左上角的点
        int col = x / kBlockSize;
        int row = y / kBlockSize;

        int leftTopPosX = kBoardMargin + kBlockSize * col;
        int leftTopPosY = kBoardMargin + kBlockSize * row;

        // 根据距离算出合适的点击位置,一共四个点，根据半径距离选最近的
        clickPosRow = -1; // 初始化最终的位置
        clickPosCol = -1;
        int len = 0; // 计算完后取整就可以了

        // 确定一个误差在范围内最近的交叉点
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta) {
            clickPosRow = row;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
        if (len < kPosDelta) {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta) {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
        if (len < kPosDelta) {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }
    //判断胜负

    //重新绘制棋盘
    update();
    ++pp;
}
void MainWindow::chessOneByPerson() {
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && clickPosRow < kBoardSizeNum && clickPosCol < kBoardSizeNum && game->getStatus(clickPosRow, clickPosCol) == 0) {
        game->setStatus(clickPosRow, clickPosCol, game->getPlayer()->getColor());
        over = game->checkWin(clickPosRow, clickPosCol);
        // 重绘
        update();

    }

}
void MainWindow::chessOneByAi() {
    setMouseTracking(false);
    std::pair<int, int > t = game->getPlayer()->moveByAi();
    game->setStatus(t.first, t.second, game->getPlayer()->getColor());
    over = game->checkWin(t.first, t.second);
    setMouseTracking(true);
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    ++pp;
    Player *now = game->getPlayer();
    if (now->getType() == 1) {//如果当前是人在下棋的话
        chessOneByPerson();
        qDebug()<<game->getPlayer()->calc(-game->getPlayer()->getColor())<<'\n';
    }

    repaint();
    if(!over){
    if(game->getPlayer()->getType() == 0){ //如果轮到AI下棋的话
        chessOneByAi();
        pp = 0;
    }
    }
}
