// Dacho2AIBeta.cpp
#include "Dacho2AIBeta.h"

// Dacho2AIBeta

// 盤面を評価
int Dacho2AIBeta::eval_board(const Board& board) {
    // 自分の石
    uint64_t player = board.player;
    // 相手の石
    uint64_t opponent = board.opponent;
    // 評価値
    int score = 0;

    // 1行目
    score += weight_LUT1[player & 0xFF] - weight_LUT1[opponent & 0xFF];
    // 2行目
    score += weight_LUT2[player >> 8 & 0xFF] - weight_LUT2[opponent >> 8 & 0xFF];
    // 3行目
    score += weight_LUT3[player >> 16 & 0xFF] - weight_LUT3[opponent >> 16 & 0xFF];
    // 4行目
    score += weight_LUT4[player >> 24 & 0xFF] - weight_LUT4[opponent >> 24 & 0xFF];
    // 5行目
    score += weight_LUT4[player >> 32 & 0xFF] - weight_LUT4[opponent >> 32 & 0xFF];
    // 6行目
    score += weight_LUT3[player >> 40 & 0xFF] - weight_LUT3[opponent >> 40 & 0xFF];
    // 7行目
    score += weight_LUT2[player >> 48 & 0xFF] - weight_LUT2[opponent >> 48 & 0xFF];
    // 8行目
    score += weight_LUT1[player >> 56 & 0xFF] - weight_LUT1[opponent >> 56 & 0xFF];

    // 評価値を返す
    return score;
}

// 手を選択
int Dacho2AIBeta::select_move(const Board& board, uint64_t legal, bool print_flag) {
    int max_score = INT_MIN, best_move = -1;
    // 合法手を探索
    while(legal) {
        // 合法手を取得
        int move = __builtin_ctzll(legal);
        // 着手
        Board tmp = board;
        tmp.reverse(move);
        // 評価値を取得
        int score = eval_board(tmp);
        // 最高評価値より高いなら更新
        if(score > max_score) {
            max_score = score;
            best_move = move;
        }
        // legalの最下位ビットを消去
        legal &= legal - 1ULL;
    }
    // 選択した手を返す
    return best_move;
}