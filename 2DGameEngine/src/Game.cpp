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

			switch (event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					isRunning = false;
				break;

				case SDLK_LEFT: case SDLK_a:
					game->MovePieceLeft();
				break;

				case  SDLK_RIGHT: case SDLK_d:
					game->MovePieceRight();
				break;

				case SDLK_DOWN: case SDLK_s:
					game->MovePieceDown();
				break;

				case SDLK_UP: case SDLK_w:
					if (!keyPressed) // restrict rotation to once per key press					
						game->RotateCurrentPiece();								
				break;

				case SDLK_g:
					if (!keyPressed)
						showGrid = !showGrid;
				break;
			}

			keyPressed = true;

		break;
		}

		case SDL_KEYUP: {

			keyPressed = false;

			//if (event.key.keysym.sym == SDLK_UP)
				//rotatePressed = false;
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
	
	SetRenderColor(Color::WHITE);

	/* draw graphics on screen */
	DrawBoard();
	DrawCurrentPiece();
	/* end of graphics draw */

	SDL_RenderPresent(renderer); // renders the frame in on the window
}

// deletes memory from heap created by SDL
void Game::Destroy()
{	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit(); // closes the program
}

void Game::SetRenderColor(Color color)
{
	switch (color)
	{
		case Color::BLACK: SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			break;

		case Color::RED: SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			break;

		case Color::LIME: SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			break;

		case Color::BLUE: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			break;

		case Color::YELLOW: SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
			break;

		case Color::MAGENTA: SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			break;

		case Color::CYAN: SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			break;

		case Color::GRAY: SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
			break;

		case Color::MAROON: SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
			break;

		case Color::GREEN: SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
			break;

		case Color::NAVY: SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
			break;

		case Color::OLIVE: SDL_SetRenderDrawColor(renderer, 128, 128, 0, 255);
			break;

		case Color::PURPLE: SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
			break;

		case Color::TEAL: SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
			break;

		case Color::WHITE: SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			break;
	}

}

void Game::DrawBoard()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	if (showGrid)
	{
		SetRenderColor(Color::MAROON);

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
	}

	// draw bordered squares
	SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255); // grey
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

}

void Game::DrawCurrentPiece()
{
	Tetromino* piece = game->GetCurrentPiece();
	int count = 0;

	// draw the current piece

	for (int y = 0; y < piece->size; y++)
	{
		for (int x = 0; x < piece->size; x++)
		{
			if (piece->tiles[y][x] == true)
			{
				tetromino[count] = { piece->posX * TILE_SIZE + (x * TILE_SIZE) + GRID_OFFSET_X,
									 piece->posY * TILE_SIZE + (y * TILE_SIZE) + GRID_OFFSET_Y + 1,
									 TILE_SIZE, TILE_SIZE };
				count++;
			}
		}
	}

	switch (piece->type)
	{
		case 0: SetRenderColor(Color::RED); break;
		case 1: SetRenderColor(Color::GREEN); break;
		case 2: SetRenderColor(Color::BLUE); break;
		case 3: SetRenderColor(Color::YELLOW); break;
		case 4: SetRenderColor(Color::MAGENTA); break;
		case 5: SetRenderColor(Color::CYAN); break;
		case 6: SetRenderColor(Color::PURPLE); break;
	}

	for (auto square : tetromino)
	{
		SDL_RenderFillRect(renderer, &square);
	}
}