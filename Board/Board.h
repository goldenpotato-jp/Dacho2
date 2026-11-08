// Board.h
#pragma once
#include <bits/stdc++.h>

// 盤面
struct Board {
    // ビットボード
    // 自分の石
    uint64_t player;
    // 相手の石
    uint64_t opponent;
    // コンストラクタ
    Board();
    // 合法手
    uint64_t get_legal() const;
    // 着手
    // posは左上を0として0~63
    void reverse(int pos);
    // パス
    void pass();
    // 終局判定
    bool is_end();
    // Perftテスト
    // 深さdepthまでテスト
    static void perft(int depth);
    // Perftテスト用再帰関数
    // 返り値はノード数
    uint64_t perft_search(int depth) const;
};