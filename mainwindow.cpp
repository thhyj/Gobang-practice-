#include "mainwindow.h"
#include<QDebug>
#include<QMessageBox>
const int kBoardMargin = 30; // 棋盘边缘空隙
const int kRadius = 15; // 棋子半径
const int kBlockSize = 40; // 格子的大小
const int kMarkSize = 6; // 落子标记边长
const int kPosDelta = 20; // 鼠标点击的模糊距离上限

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent) {
    setFixedSize(kBoardMargin * 2 + kBlockSize * (kBoardSizeNum), kBoardMargin * 2 + kBlockSize * (kBoardSizeNum));
    setMouseTracking(true);
    Board *board = new Board;
    Player *player1 = new Player(1, 1, board);
    Player *player2 = new Player(1, -1, board);

    game = new Game(board, player1, player2);
}

MainWindow::~MainWindow() {
    delete game;
}
void MainWindow::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  // 绘制棋盘
  painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
  for (int i = 0; i <= kBoardSizeNum; ++i) {
      painter.drawLine(kBoardMargin + kBlockSize * i, kBoardMargin, kBoardMargin + kBlockSize * i, size().height() - kBoardMargin);
      painter.drawLine(kBoardMargin, kBoardMargin + kBlockSize * i, size().width() - kBoardMargin, kBoardMargin + kBlockSize * i);
  }
  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  //绘制落点预测
  if (clickPosRow > 0 && clickPosRow < kBoardSizeNum &&
      clickPosCol > 0 && clickPosCol < kBoardSizeNum &&
      game->getStatus(clickPosRow, clickPosCol) == 0) {
      if (game->getPlayer()->getColor() == -1)
          brush.setColor(Qt::white);
      else
          brush.setColor(Qt::black);
      painter.setBrush(brush);
      painter.drawRect(kBoardMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBoardMargin + kBlockSize * clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
  }
  //判断胜负
  if(game->checkWin(clickPosRow, clickPosCol)) {
    setMouseTracking(false);
    qDebug() << "win";
    QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", " win!");
    delete game;
    Board *board = new Board;
    Player *player1 = new Player(1, 1, board);
    Player *player2 = new Player(1, -1, board);
    game = new Game(board, player1, player2);
    setMouseTracking(true);
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
      }
  }

}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 获取鼠标当前的坐标
    int x = event->x();
    int y = event->y();

    if (x >= kBoardMargin + kBlockSize / 2 &&
            x < size().width() - kBoardMargin &&
            y >= kBoardMargin + kBlockSize / 2 &&
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

    //重新绘制棋盘
    update();
}
void MainWindow::chessOneByPerson() {
    // 根据当前存储的坐标下子
    // 只有有效点击才下子，并且该处没有子
    if (clickPosRow != -1 && clickPosCol != -1 && game->getStatus(clickPosRow, clickPosCol) == 0) {
        game->setStatus(clickPosRow, clickPosCol, game->getPlayer()->getColor());
        // 重绘
        update();
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Player *now = game->getPlayer();
    if (now->getType() == 1) {//如果当前是人在下棋的话
        chessOneByPerson();
    }

}
