// Dacho2AIBeta.cpp
#include "Dacho2AIBeta.h"
#include "../../Const/const.h"
#include "../../Timer/Timer.h"

// Dacho2AIBeta

// コンストラクタ
Dacho2AIBeta::Dacho2AIBeta(int depth):
    // 探索深さ
    depth(depth) {}

// 盤面を評価
int Dacho2AIBeta::eval_board(const Board& board) {
    // 自分の石
    uint64_t player = board.player;
    // 相手の石
    uint64_t opponent = board.opponent;
    // 評価値
    int score = 0;

    // 1行目
    score += weight_LUT1[player & 0xFF] - weight_LUT1[opponent & 0xFF];
    // 2行目
    score += weight_LUT2[(player >> 8) & 0xFF] - weight_LUT2[(opponent >> 8) & 0xFF];
    // 3行目
    score += weight_LUT3[(player >> 16) & 0xFF] - weight_LUT3[(opponent >> 16) & 0xFF];
    // 4行目
    score += weight_LUT4[(player >> 24) & 0xFF] - weight_LUT4[(opponent >> 24) & 0xFF];
    // 5行目
    score += weight_LUT4[(player >> 32) & 0xFF] - weight_LUT4[(opponent >> 32) & 0xFF];
    // 6行目
    score += weight_LUT3[(player >> 40) & 0xFF] - weight_LUT3[(opponent >> 40) & 0xFF];
    // 7行目
    score += weight_LUT2[(player >> 48) & 0xFF] - weight_LUT2[(opponent >> 48) & 0xFF];
    // 8行目
    score += weight_LUT1[(player >> 56) & 0xFF] - weight_LUT1[(opponent >> 56) & 0xFF];

    // 評価値を返す
    return score;
}

// move ordering用の評価
int Dacho2AIBeta::eval_move_ordering(const Board& board) {  
    return __builtin_popcountll(board.get_legal());
}

// Negaalpha法用の再帰関数
int Dacho2AIBeta::search_node(const Board& board, int cur_depth, int alpha, int beta) {
    // ノード数を1増やす
    ++nodes;
    // 葉ノードなので評価値を返す
    if(cur_depth == 0) return eval_board(board);
    // 合法手を取得
    uint64_t legal = board.get_legal();
    // 自分が着手できない場合
    if(!legal) {
        // 手番交代
        Board tmp = board;
        tmp.pass();
        // 相手も着手できない場合は終局
        if(!tmp.get_legal()) {
            // 石数の差を計算
            int score = board.player_num() - board.opponent_num();
            if(score > 0) return WIN_SCORE + score;
            if(score < 0) return -WIN_SCORE + score;
            return 0;
        }
        // 相手が着手できる場合はパスした盤面を探索
        return -search_node(tmp, cur_depth - 1, -beta, -alpha);
    }
    // 残り深さが浅い場合はmove orderingをしない
    if(cur_depth <= 3) {
        // 合法手を探索
        while(legal) {
            // 合法手を取得
            int move = __builtin_ctzll(legal);
            // 着手
            Board tmp = board;
            tmp.reverse(move);
            tmp.pass();
            // 評価値を取得
            int score = -search_node(tmp, cur_depth - 1, -beta, -alpha);
            // ベータ値以上なら枝刈り
            if(score >= beta) return beta;
            // アルファ値より高いなら更新
            if(score > alpha) alpha = score;
            // legalの最下位ビットを消去
            legal &= legal - 1ULL;
        }
        // アルファ値を返す
        return alpha;
    }
    // 合法手の数
    int move_count = 0;
    // 合法手の配列
    // オセロの合法手は最大で33個
    int moves[33];
    // 合法手を探索
    while(legal) {
        // 合法手を取得
        int move = __builtin_ctzll(legal);
        // 着手
        Board tmp = board;
        tmp.reverse(move);
        tmp.pass();
        // move ordering用の評価値を取得
        int score = eval_move_ordering(tmp);
        // 合法手に追加
        moves[move_count++] = score << 16 | move;
        // legalの最下位ビットを消去
        legal &= legal - 1ULL;
    }
    // move ordering
    std::sort(moves, moves + move_count);
    // move orderingでソートされた順番で探索
    for(int i = 0; i < move_count; ++i) {
        // 探索する手
        int move = moves[i] & 0xffff;
        // 着手
        Board tmp = board;
        tmp.reverse(move);
        tmp.pass();
        // 評価値を取得
        int score = -search_node(tmp, cur_depth - 1, -beta, -alpha);
        // ベータ値以上なら枝刈り
        if(score >= beta) return beta;
        // アルファ値より高いなら更新
        if(score > alpha) alpha = score;
    }
    // アルファ値を返す
    return alpha;
}

// 手を選択
int Dacho2AIBeta::select_move(const Board& board, uint64_t legal, bool print_flag) {
    // 探索開始時間を記録
    double start = Timer::get_sec();
    // ノード数をリセット
    nodes = 0;
    // alpha: アルファ値、beta: ベータ値
    int alpha = -INF, beta = INF;
    // best_move: 暫定最善手
    int best_move = -1;
    // 合法手の数
    int move_count = 0;
    // 合法手の配列
    // オセロの合法手は最大で33個
    int moves[33];
    // 合法手を探索
    while(legal) {
        // 合法手を取得
        int move = __builtin_ctzll(legal);
        // 着手
        Board tmp = board;
        tmp.reverse(move);
        tmp.pass();
        // move ordering用の評価値を取得
        int score = eval_move_ordering(tmp);
        // 合法手に追加
        moves[move_count++] = score << 16 | move;
        // legalの最下位ビットを消去
        legal &= legal - 1ULL;
    }
    // move ordering
    std::sort(moves, moves + move_count);
    // move orderingでソートされた順番で探索
    for(int i = 0; i < move_count; ++i) {
        // 探索する手
        int move = moves[i] & 0xffff;
        // 着手
        Board tmp = board;
        tmp.reverse(move);
        tmp.pass();
        // 評価値を取得
        int score = -search_node(tmp, depth - 1, -beta, -alpha);
        // 暫定最善手がないまたはアルファ値より高いなら更新
        if(best_move == -1 || score > alpha) {
            alpha = score;
            best_move = move;
        }
    }
    // 出力
    if(print_flag) {
        // 探索終了時間を記録
        double end = Timer::get_sec();
        // 探索時間を計算
        double search_time = end - start;
        // 毎秒ノード数を計算
        double nps = nodes / search_time;
        // ノード数
        std::cout << "Nodes : " << nodes << '\n';
        // 時間
        std::cout << "Time : " << search_time << " s\n";
        // 毎秒ノード数
        std::cout << "NPS : " << nps << " nps\n";
    }
    // 選択した手を返す
    return best_move;
}

// Perftテスト
void Dacho2AIBeta::perft() {
    // テスト開始時間を記録
    double start = Timer::get_sec();
    // ノード数をリセット
    nodes = 0;
    // 盤面を初期化
    Board board;
    // テスト
    search_node(board, depth, -INF, INF);
    // テスト終了時間を記録
    double end = Timer::get_sec();
    // 探索時間を計算
    double search_time = end - start;
    // 毎秒ノード数を計算
    double nps = nodes / search_time;
    // テスト結果を出力
    std::cout << "=== Perft Test ===\n";
    // 深さ
    std::cout << "Depth : " << depth << '\n';
    // ノード数
    std::cout << "Nodes : " << nodes << '\n';
    // 時間
    std::cout << "Time : " << search_time << " s\n";
    // 毎秒ノード数
    std::cout << "NPS : " << nps << " nps\n";
}