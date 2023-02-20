#include "Game.h"
#include "Constants.h"

#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr)
{
	this->isRunning = true;

	game = new Tetris();

	game->LoadNextPiece();
}

Game::~Game()
{
	delete game;
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

		case SDL_WINDOWEVENT:
		{
			if (event.window.event == SDL_WINDOWEVENT_CLOSE)
				isRunning = false;
			break;
		}

		// sets the running state to false when the ESC key is pressed
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}

			if (event.key.keysym.sym == SDLK_LEFT)
			{				
				game->MovePieceLeft();
			}

			if (event.key.keysym.sym == SDLK_RIGHT)
			{				
				game->MovePieceRight();
			}

			if (event.key.keysym.sym == SDLK_DOWN)
			{				
				game->MovePieceDown();
			}

			if (event.key.keysym.sym == SDLK_UP)
			{				
				game->RotateCurrentPiece();
			}

			break;
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

	// draw field
	SDL_RenderDrawRect(renderer, &border);

	// draw field
	DrawBoard();

	SDL_RenderPresent(renderer); // renders the frame in on the window
}

// deletes memory from heap created by SDL
void Game::Destroy()
{	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit(); // closes the program
}



void Game::DrawBoard()
{
	int count = 0;
	Tetromino* piece = game->GetCurrentPiece();

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// hoirzontal lines
	for (int y = 0; y < ROWS; y++)
	{
		SDL_RenderDrawLine(renderer, GRID_OFFSET_X + 1, GRID_OFFSET_Y + y * TILE_SIZE,
			GRID_OFFSET_X - 1 + TILE_SIZE * COLS, GRID_OFFSET_Y + y * TILE_SIZE);
	}

	//vertical lines
	for (int x = 0; x < COLS; x++)
	{
		SDL_RenderDrawLine(renderer, GRID_OFFSET_X + x * TILE_SIZE, GRID_OFFSET_Y + 1,
			GRID_OFFSET_X + x * TILE_SIZE, GRID_OFFSET_Y - 1 + ROWS * TILE_SIZE);
	}

	// draw bordered squares
	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			if (game->GetBoardValue(x, y) == 9)
			{
				SDL_Rect borderSquare = { GRID_OFFSET_X +x * TILE_SIZE, GRID_OFFSET_Y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
				SDL_RenderFillRect(renderer, &borderSquare);
			}
		}
	}

	for (int y = 0; y < piece->size; y++)
	{
		for (int x = 0; x < piece->size; x++)
		{
			if (piece->tiles[y][x] == true)
			{
				tetromino[count] = { piece->posX * TILE_SIZE + (x * TILE_SIZE) + GRID_OFFSET_X,
									 piece->posY * TILE_SIZE + (y * TILE_SIZE) + GRID_OFFSET_Y+1,
									 TILE_SIZE, TILE_SIZE };
				count++;
			}
		}
	}

	switch (piece->type)
	{
		case 0: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); break; // red
		case 1: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); break; // green
		case 2: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break; //blue
		case 3: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); break; // yellow
		case 4: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); break; // 
		case 5: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); break; // 
		case 6: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); break; // white
	}

	for (auto square : tetromino)
	{
		SDL_RenderFillRect(renderer, &square);
	}

}