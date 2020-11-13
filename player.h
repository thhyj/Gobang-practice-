#ifndef PLAYER_H
#define PLAYER_H
#include "board.h"
#include <utility>
class Player {
private:
    int type;  // 0为bot，1为玩家
    int color; //-1为白色，1为黑色
    Board* board;
    int ma[kBoardSizeNum + 1][kBoardSizeNum + 1] = {0};
    int mp[4][kBoardSizeNum + 1][kBoardSizeNum + 1] = {0};
public:
    Player() = default;
    Player(int type, int color, Board* board);
    int getType();
    void setType(int t);
    int getColor();
    int move(int x, int y); //在x，y位置落子，返回值为1表示落子成功，为0表示落子失败
    int calc(int color);    //局面评估函数
    std::pair<int, int> moveByAi();         //由AI落子
    int AlphaBeta(int dep, int alpha, int beta, int col); //搜索
    void updateScore(int ti, int tj, int col);
    double evaluatePoint(int ti, int tj, int T);
    void evaluate();
    void evaluate1();
    void evaluate2();
    void evaluate3();
    void evaluate4();
    inline void update(int i, int j, int col);
    struct trie;            //AC自动机
};

#endif // PLAYER_H
