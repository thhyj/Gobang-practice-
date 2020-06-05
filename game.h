#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "player.h"

class Game {
private:
    Player *player[2];
    Board *board;
    int now = 0; //表示当前轮到player[0]还是player[1]
public:
    Game();
    ~Game();
    Game(Board* board, Player* player1, Player* player2);
    int getStatus(int x, int y); //获取位置x,y的棋子状态
    void setStatus(int x, int y, int v); //将位置x,y的棋子状态设置为v
    int checkWin(int y, int x);
    Player* getPlayer();//获取当前的player
    void init();
};

#endif // GAME_H
