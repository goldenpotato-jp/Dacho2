// RandomAI.h
#pragma once
#include "../../Board/Board.h"

// ランダムAI
struct RandomAI {
    // 手を選択
    static int select_move(const Board& board, bool print_flag);
};