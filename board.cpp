#include "board.h"
#include <string.h>
Board::Board() {
    memset(map, 0, sizeof(map));
}

int Board::getStatus(int x, int y) {
    return map[x][y];
}
void Board::setStatus(int x, int y, int v){
    map[x][y] = v;
    return;
}
//八个方向，由于对称性，可以只定义四个方向
//分别是上，右上，右，右下
const int tx[4] = {0, 1, 1, 1};
const int ty[4] = {-1, -1, 0, 1};
int Board::checkWin(int y, int x) {
    int now = getStatus(y, x);          //首先确定最后落子区域的棋子颜色
    if(!now) return 0;
    bool pd;
    for(int i = 0; i < 4; ++i) {            //枚举上，右上，右，右下四个方向作为正方向
        for(int j = 0; j < 5; ++j) {        //枚举当前棋子是沿着正方向的第几个棋子
            pd = 1;
            for(int k = 0 ; k <= 4 - j; ++k) {//看正方向上的棋子是否连着
                if(getStatus(y + k * ty[i], x + k * tx[i]) != now){
                   pd = 0;
                   break;
                }
            }
            for(int k = 0 ; k <= j; ++k) {     //看负方向的棋子是否连着
                if(getStatus(y - k * ty[i], x - k * tx[i]) != now){
                   pd = 0;
                   break;
                }
            }
            if(pd) return 1;
        }
    }
    return 0;
}
