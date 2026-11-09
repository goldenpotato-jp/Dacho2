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
    // 自分の石数を取得
    int player_num() const;
    // 相手の石数を取得
    int opponent_num() const;
    // 勝敗を取得
    // 1: 自分勝ち、2: 相手勝ち、0: 引き分け
    int get_result() const;
    // Perftテスト
    // 深さdepthまでテスト
    static void perft(int depth);
    // Perftテスト用再帰関数
    // 返り値はノード数
    uint64_t perft_search(int depth) const;
};