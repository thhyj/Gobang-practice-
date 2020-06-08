#include "player.h"
#include <cstring>
#include <string>
#include <queue>
#include<utility>
#include <unordered_map>
#include <QDebug>
#include <algorithm>
unsigned long long rand64() {
    return rand() ^ ((unsigned long long)rand() << 15) ^ ((unsigned long long)rand() << 30) ^ ((unsigned long long)rand() << 45) ^ ((unsigned long long)rand() << 60);
}
 std::unordered_map<unsigned long long, std::pair<int, int>>table;
 unsigned long long zob[2][15][15];
 unsigned long long hash;
struct trie {
    int son[2], fail;
    bool cnt;
    int score;
    int totscore;
} tr[605];
int tot;
int inihash;
inline void build(std::string s, int score) { //先建出一课Trie树
    int len = s.size(), temp, now = 0;
    for (int i = 0; i < len; ++i) {
        temp = s[i] - '0';
        if (!tr[now].son[temp]) {
            tr[now].son[temp] = ++tot;
        }
        now = tr[now].son[temp];
    }
    tr[now].cnt = 1;
    tr[now].score = score;
}
inline void getfail() {                //在Trie树上把Fail链给建出来，这就变成了一个AC自动机
    static std::queue<int> que;

    for (int i = 0; i < 2; ++i) {
        if (tr[0].son[i]) que.push(tr[0].son[i]);
    }
    int u, v;
    while (!que.empty()) {
        u = que.front();
        que.pop();
        for (int i = 0; i < 2; ++i) {
            v = tr[u].fail;
            if ((!tr[v].son[i]) && v) v = tr[v].fail;
            if (tr[u].son[i]) {
                tr[tr[u].son[i]].fail = tr[v].son[i];
                tr[tr[u].son[i]].cnt |= tr[tr[v].son[i]].cnt;
                que.push(tr[u].son[i]);
            } else {
                tr[u].son[i] = tr[v].son[i];
            }
        }
    }
}
inline void trieInit(){
    if(tot > 0) return;
    build("11111", 50000);
    build("011110", 4320);
    build("011100", 720);
    build("001110", 720);
    build("011010", 720);
    build("010110", 720);
    build("11110", 720);
    build("01111", 720);
    build("11011", 720);
    build("10111", 720);
    build("11101", 720);
    build("001100", 120);
    build("001010", 120);
    build("010100", 120);
    build("000100", 20);
    build("001000", 20);
    getfail();
    if(!hash) {
        for(int now  = 0; now <= tot; ++now) {
            int temp = now;
            while(temp) {
                tr[now].totscore += tr[temp].score;
                temp = tr[temp].fail;
            }
        }
        for(int i = 0; i < kBoardSizeNum; ++i ){
            for(int j = 0; j < kBoardSizeNum; ++j) {
                zob[0][i][j] = rand64();
                zob[1][i][j] = rand64();
            }
        }
        hash = rand64();
        inihash = hash;
    }
    hash = inihash;
}
inline int getScore(std::string s) {        //对于一个字符串，通过AC自动机获取它的分数
    int now = 0, temp;
    int totscore = 0;
    for (int i = 0; i < s.size(); ++i) {
        if(s[i] == '2') {
            now = 0;
            continue;
        }
        if(!tr[now].son[s[i] - '0']) now = tr[now].fail;
        now = tr[now].son[s[i] - '0'];
        totscore += tr[now].totscore;
    }
    return totscore;
}
Player::Player(int type, int color, Board *board):type(type),color(color), board(board) {
    trieInit();
}
int Player::getType(){
    return type;
}
int Player::getColor(){
    return color;
}
int Player::move(int x, int y) {
    if(x < 0 or x >= kBoardSizeNum or y >= kBoardSizeNum or y < 0) { //如果越界，落子失败
        return 0;
    }
    if(board->getStatus(x, y) != 0) {        //如果那个地方有棋子了，落子失败
        return 0;
    }
    board->setStatus(x, y, color);           //如果可以落子，那么那个地方就下一个与当前player相符的棋子
    return 1;
}
//第一维表示这是行，还是列，还是右斜还是左斜

int totalScore[2];
int hang[2][15], lie[2][15], youxie[2][30], zuoxie[2][30];
void Player::updateScore(int ti, int tj, int col) {
    std::string s;
    int type = col == 1 ? 1 : 0;
    totalScore[type] -= hang[type][ti];
    for (int i = ti, j = std::max(0, tj - 5); j < std::min(kBoardSizeNum, tj + 6); ++j) {
        s += char(ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0');  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


    }
    hang[type][ti] = getScore(s);
    totalScore[type] += hang[type][ti];
    s.clear();
    totalScore[type] -= lie[type][tj];
    for (int i = std::max(0, ti - 5), j = tj; i < std::min(kBoardSizeNum, ti + 6); ++i) {
        s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


    }
    lie[type][tj] = getScore(s);
    totalScore[type] += lie[type][tj];
    s.clear();
    totalScore[type] -= youxie[type][tj - ti + kBoardSizeNum];
    if(ti < tj) {
        for(int i = 0, j = tj - ti; j < kBoardSizeNum; ++j, ++i) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        youxie[type][tj - ti + kBoardSizeNum] = getScore(s);
        totalScore[type] += youxie[type][tj - ti + kBoardSizeNum];
        s.clear();
    } else {
        for(int j = 0, i = ti - tj; i < kBoardSizeNum; ++i, ++j) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        youxie[type][tj - ti + kBoardSizeNum] = getScore(s);
        totalScore[type] += youxie[type][tj - ti + kBoardSizeNum];
        s.clear();
    }
    totalScore[type] -= zuoxie[type][tj + ti];
    if(ti + tj < kBoardSizeNum) {
        for(int i = 0, j = ti + tj; j > 0; --j, ++i) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        zuoxie[type][tj + ti] = getScore(s);
        totalScore[type] += zuoxie[type][tj + ti];
        s.clear();
    } else {
        for(int i = ti + tj - kBoardSizeNum + 1, j = kBoardSizeNum - 1; i < kBoardSizeNum; ++i, --j) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2
        }
        zuoxie[type][tj + ti] = getScore(s);
        totalScore[type] += zuoxie[type][tj + ti];
        s.clear();
    }
}
int Player::calc(int color) {
    for (int i = 0; i < kBoardSizeNum; ++i) {
        for (int j = 0; j < kBoardSizeNum; ++j) {
            ma[i][j] = board->getStatus(i, j);
        }
    }
    int totalScore = 0;
    std::string s;
    for (int i = 0; i < kBoardSizeNum; ++i) {//计算每一行的分数

        for (int j = 0; j < kBoardSizeNum; ++j) {
            s += ma[i][j] == color ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2
        }
        totalScore += getScore(s);
        s.clear();
    }
    for (int j = 0; j < kBoardSizeNum; ++j) {//计算每一列的分数
        for (int i = 0; i < kBoardSizeNum; ++i) {
            s += ma[i][j] == color ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2
        }
        totalScore += getScore(s);
        s.clear();
    }
    //以下都是计算斜行的分数
    for(int i = 0, j = 0; j < kBoardSizeNum; ++j) {

        for(int t = 0; t + j < kBoardSizeNum; ++t) {
            s += ma[i + t][j + t] == color ? '1' : ma[i + t][j + t] ? '2' : '0';
        }
        totalScore += getScore(s);
        s.clear();
        for(int t = 0; j - t >= 0; ++t) {
            s += ma[i + t][j - t] == color ? '1' : ma[i + t][j - t] ? '2' : '0';
        }
        totalScore += getScore(s);
        s.clear();
    }
    for(int i = 1, j = 0; i < kBoardSizeNum; ++i) {
        for(int t = 0; t + i < kBoardSizeNum; ++t) {
            s += ma[i + t][j + t] == color ? '1' : ma[i + t][j + t] ? '2' : '0';
        }
        totalScore += getScore(s);
        s.clear();
    }
    for(int i = 1, j = kBoardSizeNum - 1; i < kBoardSizeNum; ++i) {
        for(int t = 0; t + i < kBoardSizeNum; ++t) {
            s += ma[i + t][j - t] == color ? '1' : ma[i + t][j - t] ? '2' : '0';
        }
        totalScore += getScore(s);
        s.clear();
    }
    return totalScore;
}

int zong = 0;
int ti, tj;
extern int Depth;
const int width = 10;
struct Choice {
    int ti, tj;
    double score;
    Choice() = default;
    Choice(int ti, int tj, int score):ti(ti), tj(tj), score(score) {
    }
    bool operator < (const Choice &rhs) const {
            return score > rhs.score;
        }
};
std::priority_queue<Choice>heap;
inline bool cmp(const Choice &a, const Choice &b) {
    return a.score > b.score;
}
int Player::predict(int col) {
    int type = col == 1 ? 1 : 0;
    int score = -0x3f3f3f3f;
    for(int i = 0; i < kBoardSizeNum; ++i) {
        for(int j = 0; j < kBoardSizeNum; ++j) {
            if(!ma[i][j]) {
                ma[i][j] = col;
                updateScore(i, j, 1);
                updateScore(i, j, -1);
                score = std::max(score, totalScore[type] - totalScore[type ^ 1]);
                ma[i][j] = 0;
                updateScore(i, j, 1);
                updateScore(i, j, -1);
            }
        }
    }
    return  -score;
}

double Player:: evaluatePoint(int ti, int tj) {
    std::string s;
    int col = 1;
    ma[ti][tj] = col;
    if(ma[ti][tj] != col) {
        qDebug()<<"fuck you !!!!!" << '\n';
    }
    int type = col == 1 ? 1 : 0;
    double  tot = 0;
    for (int i = ti, j = std::max(0, tj - 5); j < std::min(kBoardSizeNum, tj + 6); ++j) {
        s += char(ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0');  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2
    }
    tot += getScore(s);
    s.clear();
    for (int i = std::max(0, ti - 5), j = tj; i < std::min(kBoardSizeNum, ti + 6); ++i) {
        s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


    }
    tot += getScore(s);
    s.clear();
    if(ti < tj) {
        for(int i = 0, j = tj - ti; j < kBoardSizeNum; ++j, ++i) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        tot += getScore(s);
        s.clear();
    } else {
        for(int j = 0, i = ti - tj; i < kBoardSizeNum; ++i, ++j) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        tot += getScore(s);
        s.clear();
    }
    if(ti + tj < kBoardSizeNum) {
        for(int i = 0, j = ti + tj; j > 0; --j, ++i) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        tot += getScore(s);;
        s.clear();
    } else {
        for(int i = ti + tj - kBoardSizeNum + 1, j = kBoardSizeNum - 1; i < kBoardSizeNum; ++i, --j) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2
        }
        tot += getScore(s);
        s.clear();
    }
    col = -1;
    ma[ti][tj] = col;
    type = col == 1 ? 1 : 0;
    for (int i = ti, j = std::max(0, tj - 5); j < std::min(kBoardSizeNum, tj + 6); ++j) {
        s += char(ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0');  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2
    }
    tot += getScore(s);
    s.clear();
    for (int i = std::max(0, ti - 5), j = tj; i < std::min(kBoardSizeNum, ti + 6); ++i) {
        s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


    }
    tot += getScore(s);
    s.clear();
    if(ti < tj) {
        for(int i = 0, j = tj - ti; j < kBoardSizeNum; ++j, ++i) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        tot += getScore(s);
        s.clear();
    } else {
        for(int j = 0, i = ti - tj; i < kBoardSizeNum; ++i, ++j) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        tot += getScore(s);
        s.clear();
    }
    if(ti + tj < kBoardSizeNum) {
        for(int i = 0, j = ti + tj; j > 0; --j, ++i) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2


        }
        tot += getScore(s);;
        s.clear();
    } else {
        for(int i = ti + tj - kBoardSizeNum + 1, j = kBoardSizeNum - 1; i < kBoardSizeNum; ++i, --j) {
            s += ma[i][j] == col ? '1' : ma[i][j] ? '2' : '0';  //如果是自己的棋子，就为1，如果为空就为0，如果是敌方的棋子，就为2
        }
        tot += getScore(s);
        s.clear();
    }
    ma[ti][tj] = 0;
    tot -= ((double)(ti - 7) * (ti - 7) + (tj - 7) * (tj - 7)) / 100;
    return tot;
}
int Player::AlphaBeta(int dep, int alpha, int beta, int col) {
    if(dep != Depth && table.find(hash) != table.end() && table[hash].second <= dep) {
       return table[hash].first;
    }
    int type = col == 1 ? 1 : 0;
    if (dep != Depth &&totalScore[type] >= 50000) {
            return 0x3f3f3f3f - 1000 - (Depth - dep);
        }
     if (dep != Depth &&totalScore[type ^ 1] >= 50000) {

           return -0x3f3f3f3f - 1000 - (Depth - dep);
       }

    if(dep == 0) {
        ++zong;
        return totalScore[type] - totalScore[type ^ 1];
       // return calc(col) - calc(-col);
    }
    int val;
    int tot = 0;
    double score;
    for(int i = 0; i < kBoardSizeNum; ++i) {
        for(int j = 0; j < kBoardSizeNum; ++j) {
            if(!ma[i][j]) {
                score = evaluatePoint(i, j);
                if(heap.size()< width) {
                    heap.push(Choice(i, j, score));
                } else
                if(score > heap.top().score) {
                    heap.pop();
                    heap.push(Choice(i, j, score));
                }
                }
            }
        }
    Choice choice[width + 1];
    while(heap.size()) {
        choice[++tot] = heap.top();
        heap.pop();
    }
    for(int o = tot; o >= 1; --o) {
        int i = choice[o].ti;
        int j = choice[o].tj;
        if(!ma[i][j]) {
            ma[i][j] = col;
            hash ^= zob[type][i][j];
            updateScore(i, j, 1);
            updateScore(i, j, -1);
            val = -AlphaBeta(dep - 1, -beta, -alpha, -col);
            if(table.find(hash) == table.end() || table[hash].second >= dep) {
                table[hash] = std::make_pair(val, dep);
            }
            ma[i][j] = 0;
            updateScore(i, j, 1);
            updateScore(i, j, -1);
            hash ^= zob[type][i][j];
            if(val >= beta && dep != Depth) {

                return beta;
            }
            if(val > alpha) {
                alpha = val;
                if(dep == Depth) {
                     ti = i;
                     tj = j;
                }
            }
            if(dep == Depth && ti ==0 && tj == 0) {
                ti = i;
                tj = j;
            }
        }
    }
    return alpha;
}
std::pair<int, int> Player::moveByAi() {
    memset(hang, 0, sizeof (hang));
    memset(lie, 0, sizeof (lie));
    memset(youxie, 0, sizeof (youxie));
    memset(zuoxie, 0, sizeof (zuoxie));
    totalScore[1] = totalScore[0] = 0;
    for (int i = 0; i < kBoardSizeNum; ++i) {
        for (int j = 0; j < kBoardSizeNum; ++j) {
            ma[i][j] = board->getStatus(i, j);
            if(ma[i][j]) {
                updateScore(i, j, 1);
                updateScore(i, j, -1);
            }
        }
    }
  //  qDebug()<<"calc" << calc(color) << "total" << totalScore[color == 1 ? 1 : 0]<<'\n';
  //  qDebug()<<"calc" << calc(-color) << "total" << totalScore[color == 1 ? 0 : 1]<<'\n';
    int inf = 0x3f3f3f3f;
    zong = 0;
    ti = 0, tj = 0;
    this->AlphaBeta(Depth, -inf, inf, color);
    qDebug()<<"zong = "<<zong <<'\n';
    return std::pair<int, int>{ti, tj};
}
void Player::setType(int t) {
    type = t;
}
