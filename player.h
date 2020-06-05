#ifndef PLAYER_H
#define PLAYER_H
#include "board.h"

class Player {
private:
    int type;  // 0为bot，1为玩家
    int color; //-1为白色，1为黑色
    Board* board;
public:
    Player() = default;
    Player(int type, int color, Board* board);
    int getType();
    int getColor();
    int move(int x, int y); //在x，y位置落子，返回值为1表示落子成功，为0表示落子失败
};

#endif // PLAYER_H
