#include "Game.hpp"
#include "Engine.hpp"

Game game = Game();

int main() {
    Engine engine(800, 600);

    return engine.initialize(&game);
}

