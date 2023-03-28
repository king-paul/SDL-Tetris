/*#ifndef _resource_rc
#define _resource_rc

MAINICON ICON "Icon.ico";

#endif*/

#include "Constants.h"
#include "SDLApp.h"

int main(int argc, char* argv[]) 
{
    SDLApp& app = SDLApp::GetInstance();// new SDLGame(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (app.IsRunning())
    {
        app.ProcessInput();
        app.Update();
        app.Render();
    }

    app.Destroy();

    return 0;
}
