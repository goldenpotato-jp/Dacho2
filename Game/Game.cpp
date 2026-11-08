// Game.cpp
#include "Game.h"

// ゲーム

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