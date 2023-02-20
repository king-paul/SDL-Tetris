#include "Game.h"
#include "Constants.h"

#include "Tetris.h"

#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr)
{
	this->isRunning = false;	
}

Game::~Game()
{

}

bool Game::IsRunning() const
{
	return this->isRunning;
}

void Game::Initialize(int width, int height)
{
	// initiatlize SDL and ensure that it worked
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cerr << "Error Initializing SDL." << std::endl;
		return;
	}

	// create a new window
	window = SDL_CreateWindow(
		NULL, // title
		SDL_WINDOWPOS_CENTERED, // X
		SDL_WINDOWPOS_CENTERED, // Y
		width, // width
		height, // height
		SDL_WINDOW_BORDERLESS // flags
	);

	// check that window was created
	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	// create the renderer
	renderer = SDL_CreateRenderer(window, -1, 0); // -1 = default driver index

	// make sure the renderer was created
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	// if everything was successful set the running state
	isRunning = true;
	return;
}

void Game::ProcessInput()
{
	SDL_Event event;
	SDL_PollEvent(&event); 

	switch (event.type)
	{
		case SDL_QUIT: {
			isRunning = false;
			break;
		}

		// sets the running state to false when the ESC key is pressed
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
		}
		default: {
			
		}
	}
}

void Game::Update()
{
	// Wait until 16ms has ellapsed since the last frame
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

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

}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255); // set the background colour
	SDL_RenderClear(renderer); // clears the back render buffer
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // set render colour to white

	SDL_RenderPresent(renderer); // renders the frame in on the window
}

// deletes memory from heap created by SDL
void Game::Destroy()
{	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit(); // closes the program
}