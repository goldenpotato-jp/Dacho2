// Hashmap.cpp
#include "Hashmap.h"

// ハッシュマップ

// コンストラクタ
Hashmap::Hashmap(uint64_t init_cap) {
    // テーブルの大きさを2のべき乗にする
    cap = 1;
    while(cap < init_cap) cap <<= 1;
    // ビットマスクを初期化
    mask = cap - 1;
    // 要素数を初期化
    num = 0;
    // テーブルを初期化
    table = new Node[cap]();
}

// デストラクタ
Hashmap::~Hashmap() {
    // テーブルのメモリを解放
    delete[] table;
}

// 再ハッシュ
void Hashmap::rehash(uint64_t new_cap) {
    // 今のテーブル
    Node* old_table = table;
    // 今のテーブルの大きさ
    uint64_t old_cap = cap;
    // テーブルの大きさを更新
    cap = new_cap;
    // ビットマスクを更新
    mask = cap - 1;
    // テーブルのメモリ再確保
    table = new Node[cap]();
    // 要素数を初期化
    num = 0;
    // 空スロットが見つかったら挿入
    for(uint64_t i = 0; i < old_cap; ++i) if(old_table[i].key) insert_no_rehash(old_table[i].key, old_table[i].val);
    // 一時的なテーブルのメモリを解放
    delete[] old_table;
}

// 再ハッシュせずに要素を挿入
void Hashmap::insert_no_rehash(uint64_t key, uint64_t val) {
    // キーからインデックスを計算
    uint64_t idx = key & mask;
    // 空スロットが見つかるまでループ
    while(table[idx].key) {
        // キーが一致したら上書き
        if(table[idx].key == key) {
            table[idx].val = val;
            return;
        }
        // インデックスを1進める
        idx = (idx + 1) & mask;
    }
    // 空スロットに挿入
    table[idx] = {key, val};
}

// 要素を挿入（再ハッシュの可能性あり）
void Hashmap::insert(uint64_t key, uint64_t val) {
    // 負荷率が70%を超えたら再ハッシュ
    if(num > cap * 0.7) rehash(cap << 1);
    // 要素を挿入
    insert_no_rehash(key, val);
}

// アクセス
uint64_t Hashmap::find(uint64_t key) const {
    // キーからインデックスを計算
    uint64_t idx = key & mask;
    // キーが一致するまでループ
    while(table[idx].key) {
        // キーが一致したら返す
        if(table[idx].key == key) return table[idx].val;
        // インデックスを1進める
        idx = (idx + 1) & mask;
    }
    // 見つからないので無効値を返す
    return NOT_FOUND;
}