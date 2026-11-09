// Board.cpp
#include "Board.h"

// 盤面

// コンストラクタ
Board::Board():
    // 黒石の初期盤面
    player(0x0000000810000000ULL),
    // 白石の初期盤面
    opponent(0x0000001008000000ULL) {}


// 合法手
uint64_t Board::get_legal() const {
    // 空きマス
    uint64_t blank = ~(player | opponent);
    // 相手の石があり、かつ左右端でないマス
    uint64_t horizontal_watch = opponent & 0x7e7e7e7e7e7e7e7eULL;
    // 相手の石があり、かつ上下端でないマス
    uint64_t vertical_watch = opponent & 0x00FFFFFFFFFFFF00ULL;
    // 相手の石があり、かつ端でないマス
    uint64_t all_side_watch = opponent & 0x007e7e7e7e7e7e00ULL;
    // 合法手
    uint64_t legal = 0;
    // 一時的な変数
    uint64_t tmp = 0;

    // 左方向
    // 自分の石の左にある相手の石
    tmp = horizontal_watch & (player >> 1);
    // 連続した相手の石を計算する
    tmp |= horizontal_watch & (tmp >> 1);
    tmp |= horizontal_watch & (tmp >> 1);
    tmp |= horizontal_watch & (tmp >> 1);
    tmp |= horizontal_watch & (tmp >> 1);
    tmp |= horizontal_watch & (tmp >> 1);
    // 左方向の合法手
    legal |= blank & (tmp >> 1);

    // 右方向
    // 自分の石の右にある相手の石
    tmp = horizontal_watch & (player << 1);
    // 連続した相手の石を計算する
    tmp |= horizontal_watch & (tmp << 1);
    tmp |= horizontal_watch & (tmp << 1);
    tmp |= horizontal_watch & (tmp << 1);
    tmp |= horizontal_watch & (tmp << 1);
    tmp |= horizontal_watch & (tmp << 1);
    // 右方向の合法手
    legal |= blank & (tmp << 1);

    // 上方向
    // 自分の石の上にある相手の石
    tmp = vertical_watch & (player >> 8);
    // 連続した相手の石を計算する
    tmp |= vertical_watch & (tmp >> 8);
    tmp |= vertical_watch & (tmp >> 8);
    tmp |= vertical_watch & (tmp >> 8);
    tmp |= vertical_watch & (tmp >> 8);
    tmp |= vertical_watch & (tmp >> 8);
    // 上方向の合法手
    legal |= blank & (tmp >> 8);

    // 下方向
    // 自分の石の下にある相手の石
    tmp = vertical_watch & (player << 8);
    // 連続した相手の石を計算する
    tmp |= vertical_watch & (tmp << 8);
    tmp |= vertical_watch & (tmp << 8);
    tmp |= vertical_watch & (tmp << 8);
    tmp |= vertical_watch & (tmp << 8);
    tmp |= vertical_watch & (tmp << 8);
    // 下方向の合法手
    legal |= blank & (tmp << 8);

    // 左上方向
    // 自分の石の左上にある相手の石
    tmp = all_side_watch & (player >> 9);
    // 連続した相手の石を計算する
    tmp |= all_side_watch & (tmp >> 9);
    tmp |= all_side_watch & (tmp >> 9);
    tmp |= all_side_watch & (tmp >> 9);
    tmp |= all_side_watch & (tmp >> 9);
    tmp |= all_side_watch & (tmp >> 9);
    // 左上方向の合法手
    legal |= blank & (tmp >> 9);

    // 左下方向
    // 自分の石の左下にある相手の石
    tmp = all_side_watch & (player << 7);
    // 連続した相手の石を計算する
    tmp |= all_side_watch & (tmp << 7);
    tmp |= all_side_watch & (tmp << 7);
    tmp |= all_side_watch & (tmp << 7);
    tmp |= all_side_watch & (tmp << 7);
    tmp |= all_side_watch & (tmp << 7);
    // 左下方向の合法手
    legal |= blank & (tmp << 7);

    // 右上方向
    // 自分の石の右上にある相手の石
    tmp = all_side_watch & (player >> 7);
    // 連続した相手の石を計算する
    tmp |= all_side_watch & (tmp >> 7);
    tmp |= all_side_watch & (tmp >> 7);
    tmp |= all_side_watch & (tmp >> 7);
    tmp |= all_side_watch & (tmp >> 7);
    tmp |= all_side_watch & (tmp >> 7);
    // 右上方向の合法手
    legal |= blank & (tmp >> 7);

    // 右下方向
    // 自分の石の右下にある相手の石
    tmp = all_side_watch & (player << 9);
    // 連続した相手の石を計算する
    tmp |= all_side_watch & (tmp << 9);
    tmp |= all_side_watch & (tmp << 9);
    tmp |= all_side_watch & (tmp << 9);
    tmp |= all_side_watch & (tmp << 9);
    tmp |= all_side_watch & (tmp << 9);
    // 右下方向の合法手
    legal |= blank & (tmp << 9);

    // 合法手を返す
    return legal;
}

// 着手
void Board::reverse(int pos) {
    // 着手するマス
    uint64_t put = 1ULL << pos;
    // 相手の石があり、かつ左右端でないマス
    uint64_t horizontal_watch = opponent & 0x7e7e7e7e7e7e7e7eULL;
    // 相手の石があり、かつ上下端でないマス
    uint64_t vertical_watch = opponent & 0x00FFFFFFFFFFFF00ULL;
    // 相手の石があり、かつ端でないマス
    uint64_t all_side_watch = opponent & 0x007e7e7e7e7e7e00ULL;
    // 反転するマス
    uint64_t rev = 0;
    // 一時的な変数
    uint64_t tmp = 0;
    
    // 左方向
    if(horizontal_watch & (put >> 1)) {
        tmp = horizontal_watch & (put >> 1);
        tmp |= horizontal_watch & (tmp >> 1);
        tmp |= horizontal_watch & (tmp >> 1);
        tmp |= horizontal_watch & (tmp >> 1);
        tmp |= horizontal_watch & (tmp >> 1);
        tmp |= horizontal_watch & (tmp >> 1);
        if((tmp >> 1) & player) rev |= tmp;
    }

    // 右方向
    if(horizontal_watch & (put << 1)) {
        tmp = horizontal_watch & (put << 1);
        tmp |= horizontal_watch & (tmp << 1);
        tmp |= horizontal_watch & (tmp << 1);
        tmp |= horizontal_watch & (tmp << 1);
        tmp |= horizontal_watch & (tmp << 1);
        tmp |= horizontal_watch & (tmp << 1);
        if((tmp << 1) & player) rev |= tmp;
    }

    // 上方向
    if(vertical_watch & (put >> 8)) {
        tmp = vertical_watch & (put >> 8);
        tmp |= vertical_watch & (tmp >> 8);
        tmp |= vertical_watch & (tmp >> 8);
        tmp |= vertical_watch & (tmp >> 8);
        tmp |= vertical_watch & (tmp >> 8);
        tmp |= vertical_watch & (tmp >> 8);
        if((tmp >> 8) & player) rev |= tmp;
    }

    // 下方向
    if(vertical_watch & (put << 8)) {
        tmp = vertical_watch & (put << 8);
        tmp |= vertical_watch & (tmp << 8);
        tmp |= vertical_watch & (tmp << 8);
        tmp |= vertical_watch & (tmp << 8);
        tmp |= vertical_watch & (tmp << 8);
        tmp |= vertical_watch & (tmp << 8);
        if((tmp << 8) & player) rev |= tmp;
    }

    // 左上方向
    if(all_side_watch & (put >> 9)) {
        tmp = all_side_watch & (put >> 9);
        tmp |= all_side_watch & (tmp >> 9);
        tmp |= all_side_watch & (tmp >> 9);
        tmp |= all_side_watch & (tmp >> 9);
        tmp |= all_side_watch & (tmp >> 9);
        tmp |= all_side_watch & (tmp >> 9);
        if((tmp >> 9) & player) rev |= tmp;
    }

    // 左下方向
    if(all_side_watch & (put << 7)) {
        tmp = all_side_watch & (put << 7);
        tmp |= all_side_watch & (tmp << 7);
        tmp |= all_side_watch & (tmp << 7);
        tmp |= all_side_watch & (tmp << 7);
        tmp |= all_side_watch & (tmp << 7);
        tmp |= all_side_watch & (tmp << 7);
        if((tmp << 7) & player) rev |= tmp;
    }

    // 右上方向
    if(all_side_watch & (put >> 7)) {
        tmp = all_side_watch & (put >> 7);
        tmp |= all_side_watch & (tmp >> 7);
        tmp |= all_side_watch & (tmp >> 7);
        tmp |= all_side_watch & (tmp >> 7);
        tmp |= all_side_watch & (tmp >> 7);
        tmp |= all_side_watch & (tmp >> 7);
        if((tmp >> 7) & player) rev |= tmp;
    }

    // 右下方向
    if(all_side_watch & (put << 9)) {
        tmp = all_side_watch & (put << 9);
        tmp |= all_side_watch & (tmp << 9);
        tmp |= all_side_watch & (tmp << 9);
        tmp |= all_side_watch & (tmp << 9);
        tmp |= all_side_watch & (tmp << 9);
        tmp |= all_side_watch & (tmp << 9);
        if((tmp << 9) & player) rev |= tmp;
    }

    // 自分の石を置く
    player |= put;
    // 反転
    player ^= rev;
    opponent ^= rev;
}

// パス
void Board::pass() {
    // 自分と相手の石を入れ替える
    std::swap(player, opponent);
}

// 自分の石数を取得
int Board::player_num() const {
    return __builtin_popcountll(player);
}

// 相手の石数を取得
int Board::opponent_num() const {
    return __builtin_popcountll(opponent);
}

// 勝敗を取得
int Board::get_result() const {
    if(player_num() > opponent_num()) return 1;
    if(player_num() < opponent_num()) return 2;
    return 0;
}

// Perftテスト
void Board::perft(int depth) {
    // テスト開始時間を記録
    auto start = std::chrono::steady_clock::now();
    // 初期盤面を生成
    Board board;
    // 再帰関数でテスト
    uint64_t nodes = board.perft_search(depth);
    // テスト終了時間を記録
    auto end = std::chrono::steady_clock::now();
    // テスト時間を計算
    std::chrono::duration<double> elapsed = end - start;
    // 毎秒ノード数を計算
    double nps = nodes / elapsed.count();

    // テスト結果を出力
    std::cout << "=== Perft Test ===\n";
    // 深さ
    std::cout << "Depth : " << depth << '\n';
    // ノード数
    std::cout << "Nodes : " << nodes << '\n';
    // 時間
    std::cout << "Time : " << elapsed.count() << " s\n";
    // 毎秒ノード数
    std::cout << "NPS : " << nps << " nps\n";
    
}

// Perftテスト用再帰関数
uint64_t Board::perft_search(int depth) const {
    // 深さdepthまで行ったら1盤面としてカウント
    if(depth == 0) return 1ULL;
    // 合法手を計算
    uint64_t legal = get_legal();
    // 自分が着手できない場合
    if(!legal) {
        // 手番交代
        Board tmp = *this;
        tmp.pass();
        // 相手も着手できない場合は終局なので1盤面としてカウント
        if(!tmp.get_legal()) return 1ULL;
        // 相手が着手できる場合はパス
        return tmp.perft_search(depth - 1);
    }
    // 自分が着手できる場合は全合法手を探索
    uint64_t nodes = 0;
    while(legal) {
        // 次の手を計算
        int pos = __builtin_ctzll(legal);
        // 着手して手番交代
        Board tmp = *this;
        tmp.reverse(pos);
        tmp.pass();
        // ノード数を加算
        nodes += tmp.perft_search(depth - 1);
        // 最下位ビットを消去
        legal &= legal - 1ULL;
    }
    // ノード数を返す
    return nodes;
}