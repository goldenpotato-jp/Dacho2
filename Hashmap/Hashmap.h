// Hashmap.h
#pragma once
#include <bits/stdc++.h>

// ハッシュマップ
struct Hashmap {
    // スロット
    struct Node {
        // キー
        uint64_t key;
        // 値
        uint64_t val;
    };

    // 無効値としてuint64_tの最大値
    static constexpr uint64_t NOT_FOUND = UINT64_MAX;

    // テーブルの大きさ
    uint64_t cap;
    // ビットマスク
    uint64_t mask;
    // 要素数
    uint64_t num;
    // テーブル
    Node* table;

    // コンストラクタ
    Hashmap(uint64_t init_cap = 16);
    // デストラクタ
    ~Hashmap();
    // 再ハッシュ
    void rehash(uint64_t new_cap);
    // 再ハッシュせずに要素を挿入
    void insert_no_rehash(uint64_t key, uint64_t val);
    // 要素を挿入（再ハッシュの可能性あり）
    void insert(uint64_t key, uint64_t val);
    // アクセス
    uint64_t find(uint64_t key) const;
};