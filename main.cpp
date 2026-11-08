// main.cpp
#include "Game/Game.h"
#include "Players/RandomAI/RandomAI.h"
#include "Players/Human/Human.h"

int main() {
    RandomAI random;
    Human human;
    Game::play(random, human, true);
}