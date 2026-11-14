// Human.cpp
#include "Human.h"

// 人間用

// 手の文字列を位置に変換
int Human::string_to_pos(std::string s) {
    return (s[1] - '1') << 3 | (s[0] - 'A');
};

// 手を選択
int Human::select_move(const Board& board, bool print_flag) {
    // 合法手を取得
    uint64_t legal = board.get_legal();
    // 合法手を入力するまで繰り返す
    while(true) {
        // 入力を促す
        std::cout << "move ";
        // 文字列を受け取る
        std::string s;
        std::cin >> s;
        // 位置に変換
        int pos = string_to_pos(s);
        // 合法手ならば位置を返す
        if(legal >> pos & 1) return pos;
        // 合法手ではないことを出力
        std::cout << s << " isn't valid move\n";
    }
}