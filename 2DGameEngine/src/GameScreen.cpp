#include "GameScreen.h"
#include "SDLApp.h"

GameScreen::GameScreen()
{
	SDLApp& app = SDLApp::GetInstance();
	renderer = app.GetRenderer();

	// create next piece panel
	int borderWith = TILE_SIZE * 5;
	int borderHeight = TILE_SIZE * 5;
	nextPieceBorder = { MARGIN_LEFT, MARGIN_TOP + 32, borderWith, borderHeight };

	// load fonts
	arial_24 = TTF_OpenFont("assets/fonts/arial.ttf", 24);
	arial_48 = TTF_OpenFont("assets/fonts/arial.ttf", 48);	

	// create text lebels
	textLabels.emplace("nextPieceLabel", new Text(renderer, arial_24, Color::YELLOW, MARGIN_LEFT, MARGIN_TOP, "NEXT PIECE"));

	textLabels.emplace("scoreLabel", new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 64, "SCORE:"));
	textLabels.emplace("scoreValue", new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 100, MARGIN_TOP + borderHeight + 64));

	textLabels.emplace("levelLabel", new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 128, "Level:"));
	textLabels.emplace("levelValue", new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 80, MARGIN_TOP + borderHeight + 128));

	textLabels.emplace("linesLabel", new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 150, "Lines Formed:"));
	textLabels.emplace("linesValue", new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 180, MARGIN_TOP + borderHeight + 150));

	textLabels.emplace("placedLabel", new Text(renderer, arial_24, Color::LIME, MARGIN_LEFT, MARGIN_TOP + borderHeight + 182, "Pieces Placed:"));
	textLabels.emplace("placedValue", new Text(renderer, arial_24, Color::WHITE, MARGIN_LEFT + 180, MARGIN_TOP + borderHeight + 182));

	textLabels.emplace("gameOverText", new Text(renderer, arial_48, Color::RED, WINDOW_WIDTH / 2 + 50, MARGIN_TOP + borderHeight + 300, "GAME OVER!"));
	textLabels.emplace("promptUserText", new Text(renderer, arial_24, Color::YELLOW, MARGIN_LEFT, MARGIN_TOP + borderHeight + 350, "Press ESC to quit"));

	// make sure the renderer was created
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	// load images
	background = new Sprite("assets/images/metal.jpg", app.GetRenderer());

	// Load block tile sprites
	blocks.emplace("blue", new Sprite("assets/images/blue.png", renderer));
	blocks.emplace("cyan", new Sprite("assets/images/cyan.png", renderer));
	blocks.emplace("green", new Sprite("assets/images/green.png", renderer));
	blocks.emplace("orange", new Sprite("assets/images/orange.png", renderer));
	blocks.emplace("purple", new Sprite("assets/images/purple.png", renderer));
	blocks.emplace("red", new Sprite("assets/images/red.png", renderer));
	blocks.emplace("yellow", new Sprite("assets/images/yellow.png", renderer));

	// Load sound and music
	music = new Music("assets/sounds/Music.mp3");
	gameOverMusic = new Music("assets/sounds/GameOver.mp3");

	soundEffects[Event::MovePiece] = new SoundEffect("assets/sounds/move_piece.wav"); 
	soundEffects[Event::RotatePiece] = new SoundEffect("assets/sounds/rotate_piece.wav");
	soundEffects[Event::PieceLanded] = new SoundEffect("assets/sounds/piece_landed.wav");
	soundEffects[Event::LineClear] = new SoundEffect("assets/sounds/line_clear.wav");
	soundEffects[Event::Tetris4Lines] = new SoundEffect("assets/sounds/tetris_4_lines.wav");
	soundEffects[Event::LevelUp] = new SoundEffect("assets/sounds/level_up_jingle.wav");
	soundEffects[Event::GameOver] = new SoundEffect("assets/sounds/game_over.wav");

	// if everything was successful set the running state
	app.SetState(RUNNING);

	music->Play(-1);
	game = new Tetris();
	game->LoadNextPiece();
}

GameScreen::~GameScreen()
{
	delete background;

	// delete all text labels and empty map
	for (std::pair<string, Text*> text : textLabels)
	{
		delete text.second;
	}
	textLabels.clear();

	// delete all block sprites and empty map
	for (std::pair<string, Sprite*> blockColour : blocks)
	{
		delete blockColour.second;
	}
	blocks.clear();

	TTF_CloseFont(arial_24);
	TTF_CloseFont(arial_48);

	// delete all loaded sounds
	delete music;
	delete gameOverMusic;
	for (Sound* sound : soundEffects)
		delete sound;
}

void GameScreen::Update(float deltaTime)
{
	Event soundEvent = game->GetEvent();
	if (soundEvent != Null)
	{
		if (soundEvent == GameOver)
		{			
			gameOverMusic->Play(0);
			game->ResetEvent();
		}
		else
		{
			soundEffects[soundEvent]->Play();
		}
	}

	if (game->FormedLines())
	{
		game->ResetEvent();

		if (fadeCompleted)
		{
			game->ClearLinesFound();
			fadeCompleted = false;
			fadeIn = true;
		}
	}
	else if (SDLApp::GetInstance().GetState() == RUNNING)
	{
		/* Update the tetris game */
		game->Update(deltaTime);
	}	
}

void GameScreen::Render()
{
	SDLApp& app = SDLApp::GetInstance();

	app.SetRenderColor(Color::GRAY); // sets the background colour
	SDL_RenderClear(renderer); // clears the back render buffer

	background->Draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // draw background image

	// color right-hand side of window a dark grey colour
	//app.SetRenderColor({ 21, 21, 21, 255 });
	//SDL_RenderFillRect(renderer, &rightHandPanel);

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
		textLabels["gameOverText"]->Draw();
		textLabels["promptUserText"]->Draw();
	}
}

void GameScreen::HandleInput(SDL_Keycode keyPressed)
{
	SDLApp& app = SDLApp::GetInstance();;
	GameState state = SDLApp::GetInstance().GetState();

	if (keyPressed == SDLK_ESCAPE)
	{
		if(state == RUNNING)
		{
			app.SetState(PAUSED);
			music->Stop();
			std::cout << "Game Paused" << std::endl;
		}
		else if (state == PAUSED)
		{
			music->Play();
			app.SetState(RUNNING);
			std::cout << "Game Resumed" << std::endl;
		}
		if (state == GAME_OVER)
		{
			app.SetState(MAIN_MENU);
		}
	}
	else if(state == RUNNING)
	{
		switch (keyPressed)
		{
			case SDLK_p:

				break;

			case SDLK_LEFT: case SDLK_a:
					game->MovePieceLeft();
				break;

			case SDLK_RIGHT: case SDLK_d:
					game->MovePieceRight();
				break;

			case SDLK_DOWN: case SDLK_s:
					game->MovePieceDown();
				break;

			case SDLK_UP: case SDLK_w:
				if (!app.KeyPressed()) // restrict rotation to once per key press		
					game->RotateCurrentPiece(true); // clockwise rotation
				break;

			case SDLK_q:
				if (!app.KeyPressed()) // restrict rotation to once per key press
					game->RotateCurrentPiece(false); // counter clockwise rotation
				break;

			case SDLK_e:
				if (!app.KeyPressed()) // restrict rotation to once per key press
					game->RotateCurrentPiece(true); // clockwise rotation
				break;

			case SDLK_g:
				if (!app.KeyPressed())
					showGrid = !showGrid;
				break;

			case SDLK_SPACE:
				if (!app.KeyPressed())
					//game->IncreaseLevel();
					game->SendPieceToBottom();
				break;

			case SDLK_m:
				Music::ToggleMute();
			break;
		}
	}
}

void GameScreen::DrawBoard()
{
	SDLApp& app = SDLApp::GetInstance();

	app.SetRenderColor(Color::BLACK);
	SDL_RenderFillRect(renderer, &field);

	// TODO : Add Board Background

	if (showGrid)
	{
		app.SetRenderColor(Color::MAROON);

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
					case 1: blockToDraw = blocks["cyan"]; break;
					case 2: blockToDraw = blocks["purple"]; break;
					case 3: blockToDraw = blocks["yellow"]; break;
					case 4: blockToDraw = blocks["blue"]; break;
					case 5: blockToDraw = blocks["orange"]; break;
					case 6: blockToDraw = blocks["green"]; break;
					case 7: blockToDraw = blocks["red"]; break;
					case 8: app.SetRenderColor(Color::BLACK); break;
					case 9: app.SetRenderColor(Color::GRAY); break;
				}

				SDL_Rect square = { GRID_OFFSET_X + x * TILE_SIZE, GRID_OFFSET_Y + y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

				if (boardValue == 8 || boardValue == 9)
					SDL_RenderFillRect(renderer, &square);
				else
					blockToDraw->Draw(&square);

			}
		}
	}

}

void GameScreen::DrawTetromino(bool nextPiece)
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
		case 0: blockToDraw = blocks["cyan"]; break;
		case 1: blockToDraw = blocks["purple"]; break;
		case 2: blockToDraw = blocks["yellow"]; break;
		case 3: blockToDraw = blocks["blue"]; break;
		case 4: blockToDraw = blocks["orange"]; break;
		case 5: blockToDraw = blocks["green"]; break;
		case 6: blockToDraw = blocks["red"]; break;
	}

	if (nextPiece)
	{
		for (SDL_Rect square : nextTetromino)
		{
			blockToDraw->Draw(&square);
		}
	}
	else
	{
		for (SDL_Rect square : tetromino)
		{
			blockToDraw->Draw(&square);
		}
	}
}

void GameScreen::FadeLineDisplay()
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
						  TILE_SIZE * (FIELD_WIDTH - 2), TILE_SIZE };

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

void GameScreen::DrawStats()
{
	// draw text labels
	textLabels["nextPieceLabel"]->Draw();
	textLabels["scoreLabel"]->Draw();
	textLabels["levelLabel"]->Draw();
	textLabels["linesLabel"]->Draw();
	textLabels["placedLabel"]->Draw();

	/*
	for (std::pair<string, Text*> text : textLabels)
	{
		if(string::compare(text.first, string("scoreValue")) == 0)
	}*/

	// draw values next to labels
	textLabels["scoreValue"]->Draw(to_string(game->Score()).c_str());
	textLabels["levelValue"]->Draw(to_string(game->Level()).c_str());
	textLabels["linesValue"]->Draw(to_string(game->LinesFormed()).c_str());
	textLabels["placedValue"]->Draw(to_string(game->PiecesPlaced()).c_str());

	SDLApp& app = SDLApp::GetInstance();
	app.SetRenderColor(Color::WHITE);
	SDL_RenderDrawRect(renderer, &nextPieceBorder);
}