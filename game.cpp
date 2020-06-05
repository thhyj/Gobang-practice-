#include "game.h"

Game::Game() {
    board = new Board;
    player[0] = new Player;
    player[1] = new Player;
}
Game::Game(Board* board, Player* player1, Player* player2):board(board){
    player[0] = player1;
    player[1] = player2;
}
Game::~Game(){
    delete board;
    delete player[0];
    delete player[1];
}
int Game::getStatus(int x, int y) {

    return board->getStatus(x, y);
}
void Game::setStatus(int x, int y, int v){
    now ^=1;
    board->setStatus(x, y, v);
    return;
}
Player* Game::getPlayer() {
    return player[now];
}
int Game::checkWin(int y, int x) {
    if(y < 0 || x < 0 || x > 14 || y > 14) return 0;
    return board->checkWin(y, x);
}
void Game:: init() {
    delete board;
    board = new Board;
}
