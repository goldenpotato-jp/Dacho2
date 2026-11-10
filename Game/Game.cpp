// Game.cpp
#include "Game.h"

// 対局

// 手番
bool Game::turn;
// 手数
int Game::ply;
// 対局開始時間
double Game::start_time;
// 盤面
Board Game::board;
// 対局結果
GameResult Game::game_result;

// 手の位置を文字列に変換
std::string Game::pos_to_string(int pos) {
    // 列
    char col = 'A' + (pos & 7);
    // 行
    char row = '1' + (pos >> 3);
    // 列と行を繋げて返す
    return {col, row};
}

// 盤面を出力
void Game::print_board(Board board, bool turn) {
    // 手番を先手に揃える
    if(!turn) board.pass();
    // 盤面を出力
    std::cout << "  A B C D E F G H\n";
    for(int r = 0; r < 8; ++r) {
        std::cout << (r + 1);
        for(int c = 0; c < 8; ++c) {
            int pos = r << 3 | c;
            // 先手の石
            if(board.player >> pos & 1ULL) std::cout << " X";
            // 後手の石
            else if(board.opponent >> pos & 1ULL) std::cout << " O";
            // 空きマス
            else std::cout << " .";
        }
        std::cout << '\n';
    }
}

// 初期化
void Game::init() {
    // 手番を先手番に初期化
    turn = true;
    // 手数を1に初期化
    ply = 1;
    // 盤面を初期盤面に初期化
    board = Board();

}

// 対局開始時の出力
void Game::print_game_start() {
    // 対局開始を出力
    std::cout << "=== Game Start ===\n";
    // 盤面を出力
    print_board(board, turn);
}

// ターン開始時の出力
void Game::print_turn_start() {
    // 手数を出力
    std::cout << "=== Ply " << ply << " ===\n";
    // 手番を出力
    std::cout << "Player " << (turn ? 1 : 2) << " turn\n";
}

// 対局結果を取得
void Game::get_game_result() {
    // 手番を先手に揃える
    if(!turn) {
        board.pass();
        turn = true;
    }
    // 対局結果を取得
    game_result.player1_score = board.player_num();
    game_result.player2_score = board.opponent_num();
    game_result.result = board.get_result();
}

// 対局終了時の出力
void Game::print_game_over() {
    // 対局終了時間を記録
    double end_time = Timer::get_sec();
    // 対局時間を計算
    double game_time = end_time - start_time;
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
    std::cout << "Time : " << game_time << " s\n";
}