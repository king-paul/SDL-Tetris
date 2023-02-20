#include <iostream>

#include "Constants.h"
#include "Game.h"

int main(int argc, char* argv[]) 
{
    
    Game* game = new Game();

    game->Initialize(800, 600);

    while (game->IsRunning()) 
    {
        game->ProcessInput();
        game->Update();
        game->Render();
    }

    game->Destroy();

    delete game;

    return 0;
}
