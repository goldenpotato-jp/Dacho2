// Game.h
#pragma once
#include "../Board/Board.h"
#include "Timer/Timer.h"

// 対局結果
struct GameResult {
    // 先手の最終石数
    int player1_score;
    // 後手の最終石数
    int player2_score;
    // 勝敗
    // 1: 先手勝ち、2: 後手勝ち、0: 引き分け
    int result;
};

// 対局
struct Game {
    // 手番
    // 先手番かどうか
    static bool turn;
    // 手数
    static int ply;
    // 対局開始時間
    static double start_time;
    // 盤面
    static Board board;
    // 対局結果
    static GameResult game_result;
    
    // 手の位置を文字列に変換
    // posは左上を0として0~63
    // 返り値は左上をA1としてA1~H8
    static std::string pos_to_string(int pos);
    // 盤面を出力
    // turn: 先手番かどうか
    static void print_board(Board board, bool turn);
    // 初期化
    static void init();
    // 対局開始時の出力
    static void print_game_start();
    // ターン開始時の出力
    static void print_turn_start();
    // 対局結果を取得
    static void get_game_result();
    // 対局終了時の出力
    static void print_game_over();
    // 対局ループ
    // Player1: 先手、Player2: 後手
    // print_flag: 出力するかどうか
    template<typename Player1, typename Player2>
    static void game_loop(Player1& player1, Player2& player2, bool print_flag) {
        while(true) {
            // 出力
            if(print_flag) print_turn_start();
            // 合法手を取得
            uint64_t legal = board.get_legal();
            // 自分が着手できる場合
            if(legal) {
                // プレイヤーの手を取得
                int pos = -1;
                // 先手番
                if(turn) pos = player1.select_move(board, legal, print_flag);
                // 後手番
                else pos = player2.select_move(board, legal, print_flag);
                // 着手
                board.reverse(pos);
                board.pass();
                // 着手を出力
                if(print_flag) std::cout << "Player " << (turn ? 1 : 2) << " placed " << pos_to_string(pos) << '\n';
                // 手数を1増やす
                ++ply;
            }
            // 自分が着手できない場合
            else {
                // 手番交代
                Board tmp = board;
                tmp.pass();
                // 相手も着手できない場合は終局
                if(!tmp.get_legal()) break;
                // 相手が着手できる場合はパス
                board.pass();
                // パスを出力
                if(print_flag) std::cout << "Player " << (turn ? 1 : 2) << " passed\n";
            }
            // 手番交代
            turn = !turn;
            // 盤面を出力
            if(print_flag) print_board(board, turn);
        }
    }
    // 対局
    // Player1: 先手、Player2: 後手
    // print_flag: 出力するかどうか
    template<typename Player1, typename Player2>
    static GameResult play(Player1& player1, Player2& player2, bool print_flag) {
        // 対局開始時間を記録
        start_time = Timer::get_sec();
        // 初期化
        init();
        // 対局開始時の出力
        if(print_flag) print_game_start();
        // 対局ループ
        game_loop(player1, player2, print_flag);
        // 対局結果を取得
        get_game_result();
        // 出力
        if(print_flag) print_game_over();
        // 対局結果を返す
        return game_result;
    }
};