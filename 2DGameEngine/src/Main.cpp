#include <iostream>

#include "Constants.h"
#include "Game.h"

int main(int argc, char* argv[]) 
{
    Game* app = new Game();

    app->Initialize(800, 600);

    while (app->IsRunning())
    {
        app->ProcessInput();
        app->Update();
        app->Render();
    }

    app->Destroy();

    delete app;

    return 0;
}
