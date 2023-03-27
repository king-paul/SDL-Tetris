#include "SDLApp.h"
#include "Constants.h"
#include "MainMenu.h"
#include "GameScreen.h"

#include <iostream>
#include <string>

SDLApp::SDLApp(int width, int height)
{
	InitializeSDL(width, height);	

	// Start the main menu
	gameState = MAIN_MENU;
	mainMenu = new MainMenu();
}

static SDLApp instance(WINDOW_WIDTH, WINDOW_HEIGHT);

SDLApp& SDLApp::GetInstance()
{	
	return instance;
}

void SDLApp::StartGame()
{
	gameScreen = new GameScreen();
	SetState(RUNNING);
}

bool SDLApp::IsRunning() const
{
	return gameState != QUIT;
}

void SDLApp::InitializeSDL(int width, int height)
{
	// initiatlize SDL and ensure that it worked
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error Initializing SDL." << std::endl;
		return;
	}	

	// create a new window
	window = SDL_CreateWindow(
		"PaulTris", // title
		SDL_WINDOWPOS_CENTERED, // X
		SDL_WINDOWPOS_CENTERED, // Y
		width, // width
		height, // height
		SDL_WINDOW_SHOWN // flags
	);

	// check that window was created
	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	// create the renderer
	renderer = SDL_CreateRenderer(window, -1, 0); // -1 = default driver index

	// set blending mode for colour alpha channels
	SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	
	int ttf = TTF_Init();

	if (ttf == -1)
		cout << TTF_GetError() << endl;

	// initialize audio
	Mix_Init(0); // parameter 0 = wave files
	Mix_Init(MIX_INIT_MP3);

	// open and play audio
	Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNK_SIZE);
}

void SDLApp::ProcessInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
		case SDL_QUIT: {
			gameState = QUIT;
			break;
		}

		case SDL_WINDOWEVENT:
		{
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				gameState = QUIT;
			break;
		}		
				
		case SDL_KEYDOWN: {

			auto keyCode = event.key.keysym.sym;

			switch (keyCode)
			{
				case SDLK_EQUALS:
					Sound::VolumeUp(8);					
				break;

				case SDLK_MINUS:					
					Sound::VolumeDown(8);
				break;


				// if game is running, handle the move input
				default:
					if (gameScreen != nullptr)
					{
						gameScreen->HandleInput(keyCode);
					}
				break;
			}

			keyPressed = true;

		break;
		}

		case SDL_KEYUP: 
		{
			keyPressed = false;
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				//std::cout << "Mouse Pressed" << std::endl;
				leftMouseDown = true;
			}
			break;
		}

		case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				//std::cout << "Mouse Released" << std::endl;
				leftMouseDown = false;
			}
			break;
		}

		default: {
			
		}
	}
}

void SDLApp::Update()
{
	// Sleep the execution until we reach the target frame time in millisceonds
	int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);
	// Only call delay if we are too fast to prccess this frame
	if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
		SDL_Delay(timeToWait);
	}

	// Delta time is the difference in ticks from last frame converted to seconds
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	// Clamp deltaTime to a maximum value
	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

	// Sets the new ticks for the current ramt to be usind in the next pass
	ticksLastFrame = SDL_GetTicks();

	if (gameState == MAIN_MENU)
	{
		mainMenu->Update();
	}
	else if (gameState != QUIT)
	{
		if (mainMenu != nullptr)
		{
			delete mainMenu;
			mainMenu = nullptr;
		}

		gameScreen->Update(deltaTime);
	}

}

void SDLApp::Render()
{
	/* draw graphics on screen */
	if (gameState == MAIN_MENU)
	{
		mainMenu->Draw();
	}
	else if(gameState != QUIT) // on the game screen
	{
		gameScreen->Render();		
	}

	/* end of graphics draw */

	SDL_RenderPresent(renderer); // renders the frame in on the window
}

// deletes memory from heap created by SDL
void SDLApp::Destroy()
{	
	if (mainMenu != nullptr)
		delete mainMenu;

	if (gameScreen != nullptr)
		delete gameScreen;

	Mix_CloseAudio();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit(); // closes the program
}

void SDLApp::SetRenderColor(SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}