#ifndef BOARD_H
#define BOARD_H
const int kBoardSizeNum = 15; // 棋盘的长和宽
class Board {
private:
    int map[kBoardSizeNum + 1][kBoardSizeNum + 1];//表示棋盘上的棋子状态,-1为白旗，1为黑棋，0为无

public:
    Board();
    int getStatus(int x, int y); //获取位置x,y的棋子状态
    void setStatus(int x, int y, int v); //将位置x,y的棋子状态设置为v
    int checkWin(int y, int x);
};

#endif // BOARD_H
