#include "SDLGame.h"
#include "MainMenu.h"
#include "Constants.h"

#include <iostream>
#include <string>

SDLGame::SDLGame(int width, int height) : window(nullptr), renderer(nullptr)
{
	InitializeSDL(width, height);

	// load fonts
	arial_24 = TTF_OpenFont("assets/fonts/arial.ttf", 24);
	arial_48 = TTF_OpenFont("assets/fonts/arial.ttf", 48);

	if (arial_24 == nullptr || arial_48 == nullptr)
	{
		cout << TTF_GetError() << endl;
	}

	// Start the main menu

	gameState = MAIN_MENU;
	mainMenu = new MainMenu(this);

	alpha = 0;
	fadeIn = true;	
}

SDLGame::~SDLGame()
{
	if (game != nullptr)
	{
		delete game;
	}

	// free text from heap
	delete nextPieceLabel;
	delete scoreLabel;
	delete scoreValue;
	delete levelLabel;
	delete levelValue;
	delete linesLabel;
	delete linesValue;
	delete placedLabel;
	delete placedValue;
	delete gameOverText;

	// free sprites from heap
	delete blockBlue;
	delete blockCyan;
	delete blockGreen;
	delete blockOrange;
	delete blockPurple;
	delete blockRed;
	delete blockYellow;
	
	delete mainMenu;

	//delete music;

	TTF_CloseFont(arial_24);
	TTF_CloseFont(arial_48);
}

void SDLGame::StartGame()
{
	/** Create stats panel **/

	//int panelLeft = WINDOW_WIDTH / 2 + 10;

	// create next piece panel
	int borderWith = TILE_SIZE * 5;
	int borderHeight = TILE_SIZE * 5;
	nextPieceBorder = { MARGIN_LEFT, MARGIN_TOP + 32, borderWith, borderHeight };

	// create text lebels
	nextPieceLabel = new Text(renderer, arial_24, Color::YELLOW, MARGIN_LEFT, MARGIN_TOP, "NEXT PIECE");

	scoreLabel = new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 64, "SCORE:");
	scoreValue = new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 100, MARGIN_TOP + borderHeight + 64);

	levelLabel = new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 128, "Level:");
	levelValue = new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 80, MARGIN_TOP + borderHeight + 128);

	linesLabel = new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 150, "Lines Formed:");
	linesValue = new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 180, MARGIN_TOP + borderHeight + 150);

	placedLabel = new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 182, "Pieces Placed:");
	placedValue = new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 180, MARGIN_TOP + borderHeight + 182);

	gameOverText = new Text(renderer, arial_48, Color::RED, WINDOW_WIDTH / 2 + 50, MARGIN_TOP + borderHeight + 300, "GAME OVER!");
	promptUserText = new Text(renderer, arial_24, Color::YELLOW, MARGIN_LEFT, MARGIN_TOP + borderHeight + 350, "Press ESC to quit");

	// make sure the renderer was created
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	// Load block tile sprites
	blockBlue = new Sprite("assets/images/blue.png", renderer);
	blockCyan = new Sprite("assets/images/cyan.png", renderer);
	blockGreen = new Sprite("assets/images/green.png", renderer);
	blockOrange = new Sprite("assets/images/orange.png", renderer);
	blockPurple = new Sprite("assets/images/purple.png", renderer);
	blockRed = new Sprite("assets/images/red.png", renderer);
	blockYellow = new Sprite("assets/images/yellow.png", renderer);

	// Load sound and music
	music = new Music("assets/sounds/Music.mp3");
	rotateSound = new SoundEffect("assets/sounds/rotate_piece.wav");

	// if everything was successful set the running state
	gameState = RUNNING;

	music->Play();
	game = new Tetris();
	game->LoadNextPiece();
}

bool SDLGame::IsRunning() const
{
	return gameState != QUIT;
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

	// initialize audio
	Mix_Init(0);// != 0) // parameter 0 = wave files
	//{
	//	std::cout << "Mixer initialized successfully";
	//}

	Mix_Init(MIX_INIT_MP3);

	// open and play audio
	Mix_OpenAudio(AUDIO_FREQUENCY, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, AUDIO_CHUNK_SIZE);
}

void SDLGame::ProcessInput()
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

			// pause/unpause the game if ESC is pressed
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				if (gameState == RUNNING)
					gameState = PAUSED;
				else if (gameState == PAUSED)
					gameState = RUNNING;
			}

			// if game is running, handle the move input
			if (!game->GameOver())
			{
				switch (event.key.keysym.sym)
				{				
					case SDLK_ESCAPE:
						gameState = MAIN_MENU;
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
							game->RotateCurrentPiece(true); // clockwise rotation
							rotateSound->Play(); // play sound effect
						break;

					case SDLK_q:
						if (!keyPressed) // restrict rotation to once per key press					
							game->RotateCurrentPiece(false); // counter clockwise rotation
							rotateSound->Play();
						break;

					case SDLK_e:
						if (!keyPressed) // restrict rotation to once per key press					
							game->RotateCurrentPiece(true); // clockwise rotation
							rotateSound->Play();
						break;

					case SDLK_g:
						if (!keyPressed)
							showGrid = !showGrid;
						break;

					case SDLK_SPACE:
						if (!keyPressed)
							//game->IncreaseLevel();
							game->SendPieceToBottom();
						break;
				}
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
	ticksLastFrame = SDL_GetTicks();

	if (gameState == MAIN_MENU)
		mainMenu->Update();

	if (gameState == RUNNING)
	{
		if (game->FormedLines())
		{
			if (fadeCompleted)
			{
				game->ClearLinesFound();
				fadeCompleted = false;
				fadeIn = true;
			}
		}
		else if (!game->GameOver())
		{
			/* Update the tetris game */
			game->Update(deltaTime);
		}
	}

}

void SDLGame::Render()
{
	SetRenderColor(Color::GRAY); // sets the background colour
	SDL_RenderClear(renderer); // clears the back render buffer

	/* draw graphics on screen */
	if (gameState == MAIN_MENU)
	{
		mainMenu->Draw();
	}
	else if(gameState != QUIT) // on the game screen
	{
		// color right-hand side of window a dark grey colour
		SetRenderColor({ 21, 21, 21, 255 });
		SDL_RenderFillRect(renderer, &rightHandPanel);

		DrawBoard();

		if (game->FormedLines())
		{
			if (!fadeCompleted)
				FadeLineDisplay();
		}
		else
		{
			DrawTetromino(false);
		}

		DrawTetromino(true);
		DrawStats();

		// if game over occured
		if (game->GameOver())
		{
			gameOverText->Draw();
			promptUserText->Draw();
		}
	}

	/* end of graphics draw */

	SDL_RenderPresent(renderer); // renders the frame in on the window
}

// deletes memory from heap created by SDL
void SDLGame::Destroy()
{	
	Mix_CloseAudio();

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
	SetRenderColor(Color::BLACK);
	SDL_RenderFillRect(renderer, &field);

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

	Sprite* blockToDraw = nullptr;

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
					case 1: blockToDraw = blockCyan; break;
					case 2: blockToDraw = blockPurple; break;
					case 3: blockToDraw = blockYellow; break;
					case 4: blockToDraw = blockBlue; break;
					case 5: blockToDraw = blockOrange; break;
					case 6: blockToDraw = blockGreen; break;
					case 7: blockToDraw = blockRed; break;
					case 8: SetRenderColor(Color::BLACK); break;
					case 9: SetRenderColor(Color::GRAY); break;
				}

				if (boardValue == 8 || boardValue == 9)
				{
					SDL_Rect square = { GRID_OFFSET_X + x * TILE_SIZE, GRID_OFFSET_Y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
					SDL_RenderFillRect(renderer, &square);
				}
				else
				{
					blockToDraw->Draw(GRID_OFFSET_X + x * TILE_SIZE, GRID_OFFSET_Y + y * TILE_SIZE);
				}
			}
		}
	}

}

void SDLGame::DrawTetromino(bool nextPiece)
{
	Tetromino* piece = (nextPiece) ? game->GetNextPiece() : game->GetCurrentPiece();
	int count = 0;

	// create square locations on screen for tetromino
	for (int y = 0; y < piece->size; y++)
	{
		for (int x = 0; x < piece->size; x++)
		{
			if (piece->tiles[y][x] == true)
			{
				if (nextPiece)
				{
					int padding = (piece->size < 4) ? TILE_SIZE : 0;

					nextTetromino[count] = { nextPieceBorder.x + (TILE_SIZE / 2) + (x * TILE_SIZE) + padding,
											 nextPieceBorder.y + (TILE_SIZE / 2) + (y * TILE_SIZE) + padding,
											 TILE_SIZE, TILE_SIZE };
				}
				else
				{
					tetromino[count] = { piece->posX * TILE_SIZE + (x * TILE_SIZE) + GRID_OFFSET_X,
										 piece->posY * TILE_SIZE + (y * TILE_SIZE) + GRID_OFFSET_Y + 1,
										 TILE_SIZE, TILE_SIZE };
				}
				count++;
			}
		}
	}

	Sprite* blockToDraw = nullptr;

	switch (piece->type)
	{
		case 0: blockToDraw = blockCyan; break;
		case 1: blockToDraw = blockPurple; break;
		case 2: blockToDraw = blockYellow; break;
		case 3: blockToDraw = blockBlue; break;
		case 4: blockToDraw = blockOrange; break;
		case 5: blockToDraw = blockGreen; break;
		case 6: blockToDraw = blockRed; break;
	}

	if (nextPiece)
	{
		for (SDL_Rect square : nextTetromino)
		{
			blockToDraw->Draw(square.x, square.y);
		}
	}
	else
	{
		for (SDL_Rect square : tetromino)
		{			
			blockToDraw->Draw(square.x, square.y);
		}
	}
}

void SDLGame::FadeLineDisplay()
{
	if (fadeIn)
		alpha += 15;
	else
		alpha -= 15;

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