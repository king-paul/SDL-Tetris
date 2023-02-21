#include "App.h"
#include "Constants.h"

#include <iostream>
#include <string>

Game::Game() : window(nullptr), renderer(nullptr)
{
	this->isRunning = true;

	game = new Tetris();

	game->LoadNextPiece();

	alpha = 0;
	fadeIn = true;	
}

Game::~Game()
{
	delete game;

	//SDL_DestroyTexture(scoreLabel);
	delete scoreLabel;
	delete scoreValue;

	TTF_CloseFont(arial);	
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

	// set blending mode for colour alpha channels
	SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);

	// load fonts
	int ttf = TTF_Init();

	arial = TTF_OpenFont("assets/fonts/arial.ttf", 24);

	if (ttf == -1 || arial == nullptr)
	{
		cout << TTF_GetError() << endl;
	}

	// create text assets
	scoreLabel = new Text(renderer, arial, {255, 0, 0, 255}, 
						  WINDOW_WIDTH / 2 + 10, GRID_OFFSET_Y);

	scoreValue = new Text(renderer, arial, { 255, 255, 255, 255 },
						  WINDOW_WIDTH / 2 + 100, GRID_OFFSET_Y);

	/*
	// create surface pointer
	SDL_Surface* textLabel = TTF_RenderText_Blended(arial, "Score: ", { 255, 0, 0, 255 });

	// create textures from text ;labels
	scoreLabel = SDL_CreateTextureFromSurface(renderer, textLabel);

	SDL_FreeSurface(textLabel);*/


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

		case SDL_KEYUP: 
		{
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
	//ticksLastFrame = SDL_GetTicks();
	if (game->FormedLines())
	{
		//SDL_Delay(1000); // delay a bit before clearing the lines
		if (fadeCompleted)
		{
			game->ClearLinesFound();
			fadeCompleted = false;
		}
	}
	else
	{
		/* Update the tetris game */
		game->Update();
	}

	if (!game->GameRunning())
	{
		isRunning = false;
		std::cout << "Game Over!" << std::endl;
		std::cout << "Your score is " << game->Score() << std::endl;
	}

}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255); // set the background colour
	SDL_RenderClear(renderer); // clears the back render buffer
	
	SetRenderColor(Color::WHITE);

	/* draw graphics on screen */
	DrawBoard();

	if (game->FormedLines())
	{
		if(!fadeCompleted)
			FadeLineDisplay();
	}
	else
	{
		DrawCurrentPiece();
	}

	DrawStats();
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

	auto gameField = game->GetField();

	// draw coloured squares
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			int boardValue = game->GetBoardValue(x, y);

			if (boardValue != 0)
			{
				switch (boardValue)
				{
					case 0:	break;
					case 1: SetRenderColor(Color::RED); break;
					case 2: SetRenderColor(Color::GREEN); break;
					case 3: SetRenderColor(Color::BLUE); break;
					case 4: SetRenderColor(Color::YELLOW); break;
					case 5: SetRenderColor(Color::MAGENTA); break;
					case 6: SetRenderColor(Color::CYAN); break;
					case 7: SetRenderColor(Color::PURPLE); break;
					case 8: SetRenderColor(Color::BLACK); break;
					case 9: SetRenderColor(Color::GRAY); break;
				}

				SDL_Rect square = { GRID_OFFSET_X + x * TILE_SIZE, GRID_OFFSET_Y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
				SDL_RenderFillRect(renderer, &square);
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

void Game::FadeLineDisplay()
{
	SDL_Delay(1);

	if (fadeIn)
		alpha++;
	else
		alpha--;

	//std::cout << "Alpha: " << alpha << std::endl;

	auto lines = game->GetLines();

	// iterates through all rows where there is a line and fills them with a green rectangle
	for (int i = 0; i < lines.size(); i++)
	{
		SDL_Rect line = { GRID_OFFSET_X + TILE_SIZE, GRID_OFFSET_Y + lines[i] * TILE_SIZE,
						  TILE_SIZE * (FIELD_WIDTH - 2), TILE_SIZE};

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, alpha);		
		SDL_RenderFillRect(renderer, &line);
	}

	if (alpha >= 255)
	{
		fadeIn = false;
		alpha = 255;
	}

	if (!fadeIn && alpha <= 0)
	{
		fadeCompleted = true;
		alpha = 0;
	}
}

void Game::DrawStats()
{
	scoreLabel->Draw("Score: ");
	scoreValue->Draw(std::to_string(game->Score()).c_str());
}