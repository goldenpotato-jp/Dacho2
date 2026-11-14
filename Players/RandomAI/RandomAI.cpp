// RandomAI.cpp
#include "RandomAI.h"

// ランダムAI

// 手を選択
int RandomAI::select_move(const Board& board, bool print_flag) {
    // 乱数を初期化
    static std::random_device rd;
    static std::mt19937 rng(rd());
    // 合法手を取得
    uint64_t legal = board.get_legal();
    // 有効手をランダムに選択
    int popcount = __builtin_popcountll(legal);
    std::uniform_int_distribution<int> dist(0, popcount - 1);
    int target = dist(rng);
    // legalのtarget個目の1を取得
    while(target > 0) {
        // legalの最下位ビットを消去
        legal &= legal - 1ULL;
        --target;
    }
    // 選択した手を返す
    return __builtin_ctzll(legal);
}