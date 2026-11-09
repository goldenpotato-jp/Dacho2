// Game.h
#pragma once
#include "../Board/Board.h"

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

// ゲーム
struct Game {
    // 手の位置を文字列に変換
    // posは左上を0として0~63
    // 返り値は左上をA1としてA1~H8
    static std::string pos_to_string(int pos);
    // 盤面を出力
    // turn: 先手番かどうか
    static void print_board(Board board, bool turn);
    // 対局
    // Player1: 先手、Player2: 後手
    // print_flag: 対局を出力するかどうか
    template<typename Player1, typename Player2>
    static GameResult play(Player1& player1, Player2& player2, bool print_flag) {
        // 対局開始時間を記録
        auto start = std::chrono::steady_clock::now();
        // 初期盤面を生成
        Board board;
        // 手番
        // turn: 先手番かどうか
        bool turn = true;
        // 手数
        int ply = 1;
        // 出力
        if(print_flag) {
            // 対局開始を出力
            std::cout << "=== Game Start ===\n";
            // 盤面を出力
            print_board(board, turn);
        }
        // メインループ
        while(true) {
            // 出力
            if(print_flag) {
                // plyを出力
                std::cout << "=== Ply " << ply << " ===\n";
                // 手番を出力
                std::cout << "Player " << (turn ? 1 : 2) << " turn\n";
            }
            // 合法手を取得
            uint64_t legal = board.get_legal();
            // 自分が着手できない場合
            if(!legal) {
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
            // 自分が着手できる場合
            else {
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
            }
            // 手番交代
            turn = !turn;
            // 手数を1増やす
            ++ply;
            // 盤面を出力
            if(print_flag) print_board(board, turn);
        }
        // 終局
        // 手番を先手に揃える
        if(!turn) {
            board.pass();
            turn = true;
        }
        // 対局結果を取得
        GameResult game_result;
        game_result.player1_score = board.player_num();
        game_result.player2_score = board.opponent_num();
        game_result.result = board.get_result();
        // 出力
        if(print_flag) {
            // 対局終了時間を記録
            auto end = std::chrono::steady_clock::now();
            // 対局時間を計算
            std::chrono::duration<double> elapsed = end - start;
            // 対局結果を出力
            std::cout << "=== Game Over ===\n";
            // 盤面を出力
            print_board(board, turn);
            // 石数を出力
            std::cout << "Player1 score : " << game_result.player1_score << '\n';
            std::cout << "Player2 score : " << game_result.player2_score << '\n';
            // 先手勝ち
            if(game_result.player1_score > game_result.player2_score) std::cout << "Player1 win\n";
            // 後手勝ち
            else if(game_result.player1_score < game_result.player2_score) std::cout << "Player2 win\n";
            // 引き分け
            else std::cout << "Draw\n";
            // 対局時間を出力
            std::cout << "Time : " << elapsed.count() << " s\n";
        }
        // 対局結果を返す
        return game_result;
    }
};