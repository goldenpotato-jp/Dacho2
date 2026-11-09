// main.cpp
#include "Game/Game.h"
#include "Players/Dacho2AIBeta/Dacho2AIBeta.h"
#include "Players/RandomAI/RandomAI.h"
#include "Players/Human/Human.h"
#include "Test/Test.h"

int main() {
    Human human;
    // RandomAI random;
    Dacho2AIBeta dacho(8);
    Game::play(human, dacho, true);
    // Test::test(random, dacho, 100);
}