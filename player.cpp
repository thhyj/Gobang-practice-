#include "player.h"

Player::Player(int type, int color, Board *board):type(type),color(color), board(board) {}
int Player::getType(){
    return type;
}
int Player::getColor(){
    return color;
}
int Player::move(int x, int y) {
    if(x < 0 or x > kBoardSizeNum or y > kBoardSizeNum or y < 0) { //如果越界，落子失败
        return 0;
    }
    if(board->getStatus(x, y) != 0) {        //如果那个地方有棋子了，落子失败
        return 0;
    }
    board->setStatus(x, y, color);           //如果可以落子，那么那个地方就下一个与当前player相符的棋子
    return 1;
}
