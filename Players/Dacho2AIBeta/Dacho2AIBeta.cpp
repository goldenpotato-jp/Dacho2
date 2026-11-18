// Dacho2AIBeta.cpp
#include "Dacho2AIBeta.h"
#include "../../Timer/Timer.h"

// Dacho2AIBeta

// 学習データ
std::string Dacho2AIBeta::train_data_path[TRAIN_FILE_NUM] = {
    "train_data/0000000.txt",
    "train_data/0000001.txt",
    "train_data/0000002.txt",
    "train_data/0000003.txt",
    "train_data/0000004.txt",
    "train_data/0000005.txt",
    "train_data/0000006.txt",
    "train_data/0000007.txt",
    "train_data/0000008.txt",
    "train_data/0000009.txt",
    "train_data/0000010.txt",
    "train_data/0000011.txt",
    "train_data/0000012.txt",
    "train_data/0000013.txt",
    "train_data/0000014.txt",
    "train_data/0000015.txt",
    "train_data/0000016.txt",
    "train_data/0000017.txt",
    "train_data/0000018.txt",
    "train_data/0000019.txt",
    "train_data/0000020.txt",
    "train_data/0000021.txt",
    "train_data/0000022.txt",
    "train_data/0000023.txt",
    "train_data/0000024.txt",
};
Board Dacho2AIBeta::train_data_board[25000000];
int Dacho2AIBeta::train_data_answer[25000000];

// 重み
// 辺+2Xの重み
double Dacho2AIBeta::weight_edge[59049];
// 2行目の重み
double Dacho2AIBeta::weight_line1[6561];
// 3行目の重み
double Dacho2AIBeta::weight_line2[6561];
// 4行目の重み
double Dacho2AIBeta::weight_line3[6561];
// 斜めの重み
double Dacho2AIBeta::weight_diagonal[59049];

// コンストラクタ
Dacho2AIBeta::Dacho2AIBeta(int min_depth, double min_time):
    // 最低探索深さ
    min_depth(min_depth),
    // 最低探索時間
    min_time(min_time) {}

// 重みを読み込む
void Dacho2AIBeta::load_weight() {
    // ファイル入力
    std::cout << "Load weight\n";
    // 辺+2Xの重み
    std::ifstream weight_edge_file("Players/Dacho2AIBeta/weight/weight_edge.txt");
    // 2行目の重み
    std::ifstream weight_line1_file("Players/Dacho2AIBeta/weight/weight_line1.txt");
    // 3行目の重み
    std::ifstream weight_line2_file("Players/Dacho2AIBeta/weight/weight_line2.txt");
    // 4行目の重み
    std::ifstream weight_line3_file("Players/Dacho2AIBeta/weight/weight_line3.txt");
    // 斜めの重み
    std::ifstream weight_diagonal_file("Players/Dacho2AIBeta/weight/weight_diagonal.txt");
    // 読み込み失敗
    if(!weight_edge_file.is_open() || !weight_line1_file.is_open() || !weight_line2_file.is_open() || !weight_line3_file.is_open() || !weight_diagonal_file.is_open()) {
        std::cout << "Error: fail to save\n";
        return;
    }
    // 重みを読み込む
    std::string line;
    // 辺+2Xの重み
    for(int i = 0; i < 59049; ++i) {
        std::getline(weight_edge_file, line);
        weight_edge[i] = stod(line);
    }
    // 2行目の重み
    for(int i = 0; i < 6561; ++i) {
        std::getline(weight_line1_file, line);
        weight_line1[i] = stod(line);
    }
    // 3行目の重み
    for(int i = 0; i < 6561; ++i) {
        std::getline(weight_line2_file, line);
        weight_line2[i] = stod(line);
    }
    // 4行目の重み
    for(int i = 0; i < 6561; ++i) {
        std::getline(weight_line3_file, line);
        weight_line3[i] = stod(line);
    }
    // 斜めの重み
    for(int i = 0; i < 59049; ++i) {
        std::getline(weight_diagonal_file, line);
        weight_diagonal[i] = stod(line);
    }
    // 読み込み成功
    std::cout << "Load successful\n";
}

// 重みを保存
void Dacho2AIBeta::save_weight() {
    // ファイル出力
    std::cout << "Save weight\n";
    // 辺+2Xの重み
    std::ofstream weight_edge_file("Players/Dacho2AIBeta/weight/weight_edge.txt");
    // 2行目の重み
    std::ofstream weight_line1_file("Players/Dacho2AIBeta/weight/weight_line1.txt");
    // 3行目の重み
    std::ofstream weight_line2_file("Players/Dacho2AIBeta/weight/weight_line2.txt");
    // 4行目の重み
    std::ofstream weight_line3_file("Players/Dacho2AIBeta/weight/weight_line3.txt");
    // 斜めの重み
    std::ofstream weight_diagonal_file("Players/Dacho2AIBeta/weight/weight_diagonal.txt");
    // 読み込み失敗
    if(!weight_edge_file.is_open() || !weight_line1_file.is_open() || !weight_line2_file.is_open() || !weight_line3_file.is_open() || !weight_diagonal_file.is_open()) {
        std::cout << "Error: fail to save\n";
        return;
    }
    // 辺+2Xの重み
    for(int i = 0; i < 59049; ++i) weight_edge_file << weight_edge[i] << '\n';
    // 2行目の重み
    for(int i = 0; i < 6561; ++i) weight_line1_file << weight_line1[i] << '\n';
    // 3行目の重み
    for(int i = 0; i < 6561; ++i) weight_line2_file << weight_line2[i] << '\n';
    // 4行目の重み
    for(int i = 0; i < 6561; ++i) weight_line3_file << weight_line3[i] << '\n';
    // 斜めの重み
    for(int i = 0; i < 59049; ++i) weight_diagonal_file << weight_diagonal[i] << '\n';
    // 保存完了
    std::cout << "Save successful\n";
}

// 学習データを読み込む
void Dacho2AIBeta::load_train_data() {
    std::cout << "Load train data\n";
    int idx = 0;
    std::string line;
    for(int i = 0; i < TRAIN_FILE_NUM; ++i){
        std::ifstream train_data_file(train_data_path[i]);
        // 読み込み失敗
        if(!train_data_file.is_open()) {
            std::cout << "Error: fail to load\n";
            return;
        }
        // 学習データを読み込む
        for(int j = 0; j < TRAIN_DATA_SIZE; ++j) {
            std::getline(train_data_file, line);
            // 盤面に変換
            uint64_t player = 0, opponent = 0;
            for(int j = 0; j < 64; ++j) {
                if(line[j] == 'X') player |= 1ULL << j;
                else if(line[j] == 'O') opponent |= 1ULL << j;
            }
            train_data_board[idx] = Board(player, opponent);
            // 答えを取得
            train_data_answer[idx] = std::stoi(line.substr(65));
            ++idx;
        }
    }
    // 読み込み完了
    std::cout << "Load successful\n";
}

// 盤面を評価
double Dacho2AIBeta::eval_board(const Board& board) {
    // 自分の石
    uint64_t player = board.player;
    // 相手の石
    uint64_t opponent = board.opponent;
    // 評価値
    double score = 0;

    // 辺+2Xの重み（上）
    score += weight_edge[BASE_2_TO_3[(player & 0xff) | (player >> 1 & 0x100) | (player >> 5 & 0x200)] + (BASE_2_TO_3[(opponent & 0xff) | (opponent >> 1 & 0x100) | (opponent >> 5 & 0x200)] << 1)];
    // 辺+2Xの重み（下）
    score += weight_edge[BASE_2_TO_3[(player >> 56) | (player >> 41 & 0x100) | (player >> 45 & 0x200)] + (BASE_2_TO_3[(opponent >> 56) | (opponent >> 41 & 0x100) | (opponent >> 45 & 0x200)] << 1)];
    // 2行目の重み
    score += weight_line1[BASE_2_TO_3[player >> 8 & 0xff] + (BASE_2_TO_3[opponent >> 8 & 0xff] << 1)];
    // 3行目の重み
    score += weight_line2[BASE_2_TO_3[player >> 16 & 0xff] + (BASE_2_TO_3[opponent >> 16 & 0xff] << 1)];
    // 4行目の重み
    score += weight_line3[BASE_2_TO_3[player >> 24 & 0xff] + (BASE_2_TO_3[opponent >> 24 & 0xff] << 1)];
    // 5行目の重み
    score += weight_line3[BASE_2_TO_3[player >> 32 & 0xff] + (BASE_2_TO_3[opponent >> 32 & 0xff] << 1)];
    // 6行目の重み
    score += weight_line2[BASE_2_TO_3[player >> 40 & 0xff] + (BASE_2_TO_3[opponent >> 40 & 0xff] << 1)];
    // 7行目の重み
    score += weight_line1[BASE_2_TO_3[player >> 48 & 0xff] + (BASE_2_TO_3[opponent >> 48 & 0xff] << 1)];
    // 対角線反転
    player = Board::transpose(player);
    opponent = Board::transpose(opponent);
    // 辺+2Xの重み（左）
    score += weight_edge[BASE_2_TO_3[(player & 0xff) | (player >> 1 & 0x100) | (player >> 5 & 0x200)] + (BASE_2_TO_3[(opponent & 0xff) | (opponent >> 1 & 0x100) | (opponent >> 5 & 0x200)] << 1)];
    // 辺+2Xの重み（右）
    score += weight_edge[BASE_2_TO_3[(player >> 56) | (player >> 41 & 0x100) | (player >> 45 & 0x200)] + (BASE_2_TO_3[(opponent >> 56) | (opponent >> 41 & 0x100) | (opponent >> 45 & 0x200)] << 1)];
    // 2列目の重み
    score += weight_line1[BASE_2_TO_3[player >> 8 & 0xff] + (BASE_2_TO_3[opponent >> 8 & 0xff] << 1)];
    // 3列目の重み
    score += weight_line2[BASE_2_TO_3[player >> 16 & 0xff] + (BASE_2_TO_3[opponent >> 16 & 0xff] << 1)];
    // 4列目の重み
    score += weight_line3[BASE_2_TO_3[player >> 24 & 0xff] + (BASE_2_TO_3[opponent >> 24 & 0xff] << 1)];
    // 5列目の重み
    score += weight_line3[BASE_2_TO_3[player >> 32 & 0xff] + (BASE_2_TO_3[opponent >> 32 & 0xff] << 1)];
    // 6列目の重み
    score += weight_line2[BASE_2_TO_3[player >> 40 & 0xff] + (BASE_2_TO_3[opponent >> 40 & 0xff] << 1)];
    // 7列目の重み
    score += weight_line1[BASE_2_TO_3[player >> 48 & 0xff] + (BASE_2_TO_3[opponent >> 48 & 0xff] << 1)];

    // 評価値を返す
    return score;
}

// 学習
void Dacho2AIBeta::learn(int idx_start, int idx_end, double learning_rate) {
    // 学習
    for(int i = idx_start; i < idx_end; ++i) {
        // 盤面を取得
        uint64_t player = train_data_board[i].player, opponent = train_data_board[i].opponent;
        // 評価値を取得
        double eval_score = eval_board(train_data_board[i]);
        // 答えを取得
        int answer_score = train_data_answer[i];
        // 勾配を計算
        double gradient = (eval_score - answer_score) * learning_rate;

        // 辺+2Xの重み（上）
        weight_edge[BASE_2_TO_3[(player & 0xff) | (player >> 1 & 0x100) | (player >> 5 & 0x200)] + (BASE_2_TO_3[(opponent & 0xff) | (opponent >> 1 & 0x100) | (opponent >> 5 & 0x200)] << 1)] -= gradient;
        // 辺+2Xの重み（下）
        weight_edge[BASE_2_TO_3[(player >> 56) | (player >> 41 & 0x100) | (player >> 45 & 0x200)] + (BASE_2_TO_3[(opponent >> 56) | (opponent >> 41 & 0x100) | (opponent >> 45 & 0x200)] << 1)] -= gradient;
        // 2行目の重み
        weight_line1[BASE_2_TO_3[player >> 8 & 0xff] + (BASE_2_TO_3[opponent >> 8 & 0xff] << 1)] -= gradient;
        // 3行目の重み
        weight_line2[BASE_2_TO_3[player >> 16 & 0xff] + (BASE_2_TO_3[opponent >> 16 & 0xff] << 1)] -= gradient;
        // 4行目の重み
        weight_line3[BASE_2_TO_3[player >> 24 & 0xff] + (BASE_2_TO_3[opponent >> 24 & 0xff] << 1)] -= gradient;
        // 5行目の重み
        weight_line3[BASE_2_TO_3[player >> 32 & 0xff] + (BASE_2_TO_3[opponent >> 32 & 0xff] << 1)] -= gradient;
        // 6行目の重み
        weight_line2[BASE_2_TO_3[player >> 40 & 0xff] + (BASE_2_TO_3[opponent >> 40 & 0xff] << 1)] -= gradient;
        // 7行目の重み
        weight_line1[BASE_2_TO_3[player >> 48 & 0xff] + (BASE_2_TO_3[opponent >> 48 & 0xff] << 1)] -= gradient;
        // 対角線反転
        player = Board::transpose(player);
        opponent = Board::transpose(opponent);
        // 辺+2Xの重み（左）
        weight_edge[BASE_2_TO_3[(player & 0xff) | (player >> 1 & 0x100) | (player >> 5 & 0x200)] + (BASE_2_TO_3[(opponent & 0xff) | (opponent >> 1 & 0x100) | (opponent >> 5 & 0x200)] << 1)] -= gradient;
        // 辺+2Xの重み（右）
        weight_edge[BASE_2_TO_3[(player >> 56) | (player >> 41 & 0x100) | (player >> 45 & 0x200)] + (BASE_2_TO_3[(opponent >> 56) | (opponent >> 41 & 0x100) | (opponent >> 45 & 0x200)] << 1)] -= gradient;
        // 2列目の重み
        weight_line1[BASE_2_TO_3[player >> 8 & 0xff] + (BASE_2_TO_3[opponent >> 8 & 0xff] << 1)] -= gradient;
        // 3列目の重み
        weight_line2[BASE_2_TO_3[player >> 16 & 0xff] + (BASE_2_TO_3[opponent >> 16 & 0xff] << 1)] -= gradient;
        // 4列目の重み
        weight_line3[BASE_2_TO_3[player >> 24 & 0xff] + (BASE_2_TO_3[opponent >> 24 & 0xff] << 1)] -= gradient;
        // 5列目の重み
        weight_line3[BASE_2_TO_3[player >> 32 & 0xff] + (BASE_2_TO_3[opponent >> 32 & 0xff] << 1)] -= gradient;
        // 6列目の重み
        weight_line2[BASE_2_TO_3[player >> 40 & 0xff] + (BASE_2_TO_3[opponent >> 40 & 0xff] << 1)] -= gradient;
        // 7列目の重み
        weight_line1[BASE_2_TO_3[player >> 48 & 0xff] + (BASE_2_TO_3[opponent >> 48 & 0xff] << 1)] -= gradient;
    }
}

// move ordering用の評価
int Dacho2AIBeta::eval_move_ordering(const Board& board) {
    uint64_t val = prev_transpose_table.find(board.hash());
    if(val != Hashmap::NOT_FOUND) return CACHE_HIT_BONUS + val;
    return eval_board(board);
}

// 評価テスト
void Dacho2AIBeta::test_eval(int idx_start, int idx_end) {
    // テスト開始時間を記録
    double start = Timer::get_sec();

    // 評価関数をテスト
    double loss_sum = 0;
    for(int i = idx_start; i < idx_end; ++i) {
        // 評価値を取得
        int eval_score = eval_board(train_data_board[i]);
        // 答えを取得
        int answer_score = train_data_answer[i];
        // 損失を計算
        int loss = (eval_score - answer_score) * (eval_score - answer_score);
        // 損失を加算
        loss_sum += loss;
    }

    // テスト終了時間を記録
    double end = Timer::get_sec();
    // テスト時間を計算
    double test_time = end - start;
    // テスト数を計算
    int test_num = idx_end - idx_start;
    // 毎秒テスト数を計算
    double tps = test_num / test_time;
    // 平均損失を計算
    double loss_avg = loss_sum / test_num;
    
    // テスト結果を出力
    std::cout << "=== Eval Test ===\n";
    // テスト数
    std::cout << "Test num : " << test_num << '\n';
    // 時間
    std::cout << "Time : " << test_time << " s\n";
    // 毎秒テスト数
    std::cout << "TPS : " << tps << " tps\n";
    // 平均損失
    std::cout << "Loss avg: " << loss_avg << '\n';
}

// Negaalpha法
int Dacho2AIBeta::nega_alpha(const Board& board, int depth, int alpha, int beta) {
    // ノード数を1増やす
    ++nodes;

    // 葉ノードならば評価値を返す
    if(depth == 0) return eval_board(board);

    // 置換表枝刈り
    uint64_t hash = board.hash(), val = transpose_table.find(hash);
    if(val != Hashmap::NOT_FOUND) return val;

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
        return -nega_alpha(tmp, depth, -beta, -alpha);
    }

    // 残り深さが浅い場合はmove orderingをしない
    if(depth <= 3) {
        // 合法手を探索
        while(legal) {
            // 合法手を取得
            int move = __builtin_ctzll(legal);
            // 着手
            Board tmp = board;
            tmp.reverse(move);
            tmp.pass();
            // 評価値を取得
            int score = -nega_alpha(tmp, depth - 1, -beta, -alpha);
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
    if(move_count >= 2) std::sort(moves, moves + move_count);
    // move orderingでソートされた順番で探索
    int max_score = -INF;
    for(int i = 0; i < move_count; ++i) {
        // 探索する手
        int move = moves[i] & 0xffff;
        // 着手
        Board tmp = board;
        tmp.reverse(move);
        tmp.pass();
        // 評価値を取得
        int score = -nega_alpha(tmp, depth - 1, -beta, -alpha);
        // ベータ値以上なら枝刈り
        if(score >= beta) return beta;
        // アルファ値より高いなら更新
        if(score > alpha) alpha = score;
        // 最大評価値より高いなら更新
        if(score > max_score) max_score = score;
    }
    
    // 置換表に登録
    transpose_table.insert(hash, max_score);

    // 最大評価値を返す
    return max_score;
}

// 手を選択
int Dacho2AIBeta::select_move(const Board& board, bool print_flag) {
    // 探索開始時間を記録
    double start = Timer::get_sec();
    // ノード数を初期化
    nodes = 0;
    // 置換表を初期化
    transpose_table.clear();
    prev_transpose_table.clear();
    // 暫定最善手
    int best_move = -1, board_score;
    // 今の深さ
    int depth = std::max(1, min_depth - 3);

    // 反復深化
    while(true) {
        // alpha: アルファ値、beta: ベータ値
        int alpha = -INF, beta = INF;
        // 合法手の数
        int move_count = 0;
        // 合法手の配列
        // オセロの合法手は最大で33個
        int moves[33];

        // 合法手を取得
        uint64_t legal = board.get_legal();
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
            int score = -nega_alpha(tmp, depth - 1, -beta, -alpha);
            // 評価値がアルファ値より高いなら更新
            if(score > alpha) {
                alpha = score;
                best_move = move;
            }
        }

        // 評価値を記録
        board_score = alpha;
        // 現在の時間を記録
        double cur_time = Timer::get_sec();
        // 探索時間を計算
        double search_time = cur_time - start;
        // 最低探索深さと最低探索時間を超えたなら終了
        if(depth >= min_depth && search_time > min_time) break;
        // 探索深さが60なら終了
        if(depth == 60) break;
        // 探索深さを1増やす
        ++depth;
        // 置換表を入れ替える
        transpose_table.swap(prev_transpose_table);
        transpose_table.clear();
    }

    // 出力
    if(print_flag) {
        // 探索終了時間を記録
        double end = Timer::get_sec();
        // 探索時間を計算
        double search_time = end - start;
        // 毎秒ノード数を計算
        double nps = nodes / search_time;
        // 探索深さ
        std::cout << "Depth : " << depth << '\n';
        // ノード数
        std::cout << "Nodes : " << nodes << '\n';
        // 時間
        std::cout << "Time : " << search_time << " s\n";
        // 毎秒ノード数
        std::cout << "NPS : " << nps << " nps\n";
        // 評価値
        std::cout << "Score: " << board_score << '\n';
    }
    // 選択した手を返す
    return best_move;
}

// 探索テスト
void Dacho2AIBeta::test_search() {
    // テスト開始時間を記録
    double start = Timer::get_sec();
    // ノード数を初期化
    nodes = 0;
    // テスト
    int score = nega_alpha(Board(), min_depth, -INF, INF);
    // テスト終了時間を記録
    double end = Timer::get_sec();
    // 探索時間を計算
    double search_time = end - start;
    // 毎秒ノード数を計算
    double nps = nodes / search_time;

    // テスト結果を出力
    std::cout << "=== Search Test ===\n";
    // 深さ
    std::cout << "Depth : " << min_depth << '\n';
    // ノード数
    std::cout << "Nodes : " << nodes << '\n';
    // 時間
    std::cout << "Time : " << search_time << " s\n";
    // 毎秒ノード数
    std::cout << "NPS : " << nps << " nps\n";
    // 評価値
    std::cout << "Score: " << score << '\n';
}