// main.cpp
#include "Game/Game.h"
#include "Players/Dacho2AIBeta/Dacho2AIBeta.h"
#include "Players/RandomAI/RandomAI.h"
#include "Players/Human/Human.h"

int main() {
    RandomAI random;
    Dacho2AIBeta dacho;
    Game::play(random, dacho, true);
}