#include "Constants.h"
#include "SDLApp.h"

int main(int argc, char* argv[]) 
{
    SDLApp& app = SDLApp::GetInstance();

    while (app.IsRunning())
    {
        app.ProcessInput();
        app.Update();
        app.Render();
    }

    app.Destroy();

    return 0;
}
