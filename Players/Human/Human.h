// Human.h
#pragma once
#include "../../Board/Board.h"

// 人間用
// 標準入出力で対局できる
struct Human {
    // 手の文字列を位置に変換
    // sは左上をA1としてA1~H8
    // 返り値は左上を0として0~63
    static int string_to_pos(std::string s);
    // 手を選択
    static int select_move(const Board& board, bool print_flag);
};