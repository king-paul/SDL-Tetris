#include "SDLApp.h"
#include "Constants.h"
#include "MainMenu.h"
#include "GameScreen.h"

#include <iostream>
#include <string>

SDLApp::SDLApp(int width, int height)
{
	InitializeSDL(width, height);	

	// load fonts
	//arial_24 = TTF_OpenFont("assets/fonts/arial.ttf", 24);
	//arial_48 = TTF_OpenFont("assets/fonts/arial.ttf", 48);

	/*
	if (arial_24 == nullptr || arial_48 == nullptr)
	{
		cout << TTF_GetError() << endl;
	}*/

	// Start the main menu

	gameState = MAIN_MENU;
	mainMenu = new MainMenu();

	//alpha = 0;
	//fadeIn = true;	
}

SDLApp::~SDLApp()
{
	/*
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
	
	delete mainMenu;*/

	//delete music;

	//TTF_CloseFont(arial_24);
	//TTF_CloseFont(arial_48);
}

static SDLApp instance(WINDOW_WIDTH, WINDOW_HEIGHT);

SDLApp& SDLApp::GetInstance()
{	
	return instance;
}

void SDLApp::StartGame()
{
	/** Create stats panel **/

	//int panelLeft = WINDOW_WIDTH / 2 + 10;

	// create next piece panel
	/*
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
	*/

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

			// pause/unpause the game if ESC is pressed
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				if (gameState == RUNNING)
					gameState = PAUSED;
				else if (gameState == PAUSED)
					gameState = RUNNING;
			}

			// if game is running, handle the move input
			if (gameScreen != nullptr)
			{
				gameScreen->HandleInput(event.key.keysym.sym);

				/*
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
				}*/
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

		/*
		SetRenderColor(Color::GRAY); // sets the background colour
		SDL_RenderClear(renderer); // clears the back render buffer

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
		}*/
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