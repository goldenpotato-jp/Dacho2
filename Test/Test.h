// Test.h
#pragma once
#include "../Game/Game.h"

// テスト
struct Test {
    // テスト
    // player1が先手、player2が後手をfloor(game_num / 2)局
    // player2が先手、player1が後手をfloor((game_num + 1) / 2)局
    // 合計game_num局テストする
    template<typename Player1, typename Player2>
    static void test(Player1& player1, Player2& player2, int game_num) {
        // テスト開始時間を記録
        auto start = std::chrono::steady_clock::now();
        // player1_win: player1勝ち、player2_win: player2勝ち、draw: 引き分け
        int player1_win = 0, player2_win = 0, draw = 0;

        // player1が先手、player2が後手をgame_num / 2局
        for(int i = 0; i < game_num / 2; ++i) {
            // 対局結果を取得
            GameResult game_result = Game::play(player1, player2, false);
            // player1勝ちの場合
            if(game_result.result == 1) ++player1_win;
            // player2勝ちの場合
            else if(game_result.result == 2) ++player2_win;
            // 引き分けの場合
            else ++draw;
        }
        // player2が先手、player1が後手を(game_num + 1) / 2局
        for(int i = 0; i < (game_num + 1) / 2; ++i) {
            // 対局結果を取得
            GameResult game_result = Game::play(player2, player1, false);
            // player1勝ちの場合
            if(game_result.result == 1) ++player2_win;
            // player2勝ちの場合
            else if(game_result.result == 2) ++player1_win;
            // 引き分けの場合
            else ++draw;
        }

        // 対局終了時間を記録
        auto end = std::chrono::steady_clock::now();
        // 対局時間を計算
        std::chrono::duration<double> elapsed = end - start;
        
        // テスト結果を出力
        std::cout << "=== Test Result ===\n";
        // テスト対局数
        std::cout << "game num : " << game_num << '\n';
        // player1の勝数
        std::cout << "player1 win : " << player1_win << '\n';
        // player2の勝数
        std::cout << "player2 win : " << player2_win << '\n';
        // 引き分け数
        std::cout << "draw : " << draw << '\n';
        // player1の勝率
        std::cout << "player1 win rate : " << (double)player1_win / game_num << '\n';
        // player2の勝率
        std::cout << "player2 win rate : " << (double)player2_win / game_num << '\n';
        // 引き分け率
        std::cout << "draw rate : " << (double)draw / game_num << '\n';
        // テスト時間を出力
        std::cout << "test time : " << elapsed.count() << " s\n";
    }
};