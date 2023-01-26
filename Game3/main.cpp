#include "Game.h"

#include <time.h>

int main()
{

    srand(time(NULL));

    Game game;
    
    game.run();

    return 0;
}