#include <iostream>

#include "Constants.h"
#include "SDLGame.h"

int main(int argc, char* argv[]) 
{
    SDLGame* app = new SDLGame(WINDOW_WIDTH, WINDOW_HEIGHT);

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
