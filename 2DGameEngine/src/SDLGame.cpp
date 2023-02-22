#include "SDLGame.h"
#include "Constants.h"

#include <iostream>
#include <string>

SDLGame::SDLGame(int width, int height) : window(nullptr), renderer(nullptr)
{
	InitializeSDL(width, height);

	// load fonts
	arial = TTF_OpenFont("assets/fonts/arial.ttf", 24);

	if (arial == nullptr)
	{
		cout << TTF_GetError() << endl;
	}

	/************************
	 ** Create stats panel **
	 ************************/
	int panelLeft = WINDOW_WIDTH / 2 + 10;

	// create next piece panel
	int borderWith = TILE_SIZE * 5;
	int borderHeight = TILE_SIZE * 5;
	nextPieceBorder = { panelLeft, GRID_OFFSET_Y + 32, borderWith, borderHeight };

	// create text lebels
	nextPieceLabel = new Text(renderer, arial, Color::YELLOW, panelLeft, GRID_OFFSET_Y, "NEXT PIECE");

	scoreLabel = new Text(renderer, arial, Color::RED, panelLeft, GRID_OFFSET_Y + borderHeight + 64, "SCORE:");
	scoreValue = new Text(renderer, arial, Color::WHITE, panelLeft + 100, GRID_OFFSET_Y + borderHeight + 64);	

	levelLabel = new Text(renderer, arial, Color::RED, panelLeft, GRID_OFFSET_Y + borderHeight + 128, "Level:");
	levelValue = new Text(renderer, arial, Color::WHITE, panelLeft + 80, GRID_OFFSET_Y + borderHeight + 128);

	linesLabel = new Text(renderer, arial, Color::RED, panelLeft, GRID_OFFSET_Y + borderHeight + 150, "Lines Formed:");
	linesValue = new Text(renderer, arial, Color::WHITE, panelLeft + 180, GRID_OFFSET_Y + borderHeight + 150);

	placedLabel = new Text(renderer, arial, Color::RED, panelLeft, GRID_OFFSET_Y + borderHeight + 182, "Pieces Placed:");
	placedValue = new Text(renderer, arial, Color::WHITE, panelLeft + 180, GRID_OFFSET_Y + borderHeight + 182);

	// make sure the renderer was created
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	// if everything was successful set the running state
	isRunning = true;


	game = new Tetris();

	game->LoadNextPiece();

	alpha = 0;
	fadeIn = true;	
}

SDLGame::~SDLGame()
{
	delete game;

	//SDL_DestroyTexture(scoreLabel);
	delete scoreLabel;
	delete scoreValue;

	TTF_CloseFont(arial);	
}

bool SDLGame::IsRunning() const
{
	return this->isRunning;
}

void SDLGame::InitializeSDL(int width, int height)
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
}

void SDLGame::ProcessInput()
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

void SDLGame::Update()
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

void SDLGame::Render()
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
void SDLGame::Destroy()
{	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit(); // closes the program
}

void SDLGame::SetRenderColor(SDL_Color color)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void SDLGame::DrawBoard()
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

void SDLGame::DrawCurrentPiece()
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

void SDLGame::FadeLineDisplay()
{
	SDL_Delay(1);

	if (fadeIn)
		alpha++;
	else
		alpha--;

	//std::cout << "Alpha: " << alpha << std::endl;

	auto lines = game->GetLines();

	// iterates through all rows where there is a line and fills them with a green rectangle
	for (unsigned int i = 0; i < lines.size(); i++)
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

void SDLGame::DrawStats()
{
	// draw text labels
	nextPieceLabel->Draw();
	scoreLabel->Draw();
	levelLabel->Draw();
	linesLabel->Draw();
	placedLabel->Draw();

	// draw values next to labels
	scoreValue->Draw(to_string(game->Score()).c_str());
	levelValue->Draw(to_string(game->Level()).c_str());
	linesValue->Draw(to_string(game->LinesFormed()).c_str());
	placedValue->Draw(to_string(game->PiecesPlaced()).c_str());

	SetRenderColor(Color::WHITE);
	SDL_RenderDrawRect(renderer, &nextPieceBorder);
}