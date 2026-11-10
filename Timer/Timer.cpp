// Timer.cpp
#include "Timer.h"

// タイマー

// 現在の時間を取得
double Timer::get_sec() {
    return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
}