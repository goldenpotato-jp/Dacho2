// main.cpp
#include "Game/Game.h"
#include "Players/Dacho2AIBeta/Dacho2AIBeta.h"
#include "Players/RandomAI/RandomAI.h"
#include "Players/Human/Human.h"
#include "Test/Test.h"

int main() {
    Human human;
    Dacho2AIBeta dacho;
    Game::play(dacho, human, true);
}