// main.cpp
#include "Game/Game.h"
#include "Players/RandomAI/RandomAI.h"

int main() {
    RandomAI random;
    Game::play(random, random, true);
}