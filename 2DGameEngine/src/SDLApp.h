#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Tetris.h"
#include "Constants.h"
#include "Text.h"
#include "Colors.h"
#include "Sound.h"
#include "sprite.h"

class MainMenu;
class GameScreen;

enum GameState
{
	MAIN_MENU,
	RUNNING,
	PAUSED,
	GAME_OVER,
	QUIT
};

class SDLApp 
{
public:
	SDLApp(int width, int height);
	~SDLApp();

	static SDLApp& GetInstance();

	bool IsRunning() const;

	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	void StartGame();

	// getters
	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }
	bool KeyPressed() { return keyPressed; }
	bool MousePressed() { return leftMouseDown; }

	// setters
	void SetState(GameState state) { gameState = state; }
	//void SetRenderColor(ColorName color);
	void SetRenderColor(SDL_Color color);

	// variables
	int ticksLastFrame = 0;

private:

	GameState gameState;
	SDL_Window* window;
	SDL_Renderer* renderer;
	MainMenu* mainMenu;
	GameScreen* gameScreen;

	// input variables
	bool keyPressed = false;
	bool leftMouseDown = false;

	//Tetris* game;

	// game objects	
	/*
	SDL_Rect tetromino[4];
	SDL_Rect nextTetromino[4];

	// panels
	SDL_Rect field = { GRID_OFFSET_X, GRID_OFFSET_Y, COLS * TILE_SIZE, ROWS * TILE_SIZE };
	SDL_Rect rightHandPanel{ WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT };
	SDL_Rect nextPieceBorder;

	// block sprites
	Sprite* blockBlue;
	Sprite* blockCyan;
	Sprite* blockGreen;
	Sprite* blockOrange;
	Sprite* blockPurple;
	Sprite* blockRed;
	Sprite* blockYellow;	

	// drawing options
	bool showGrid = false;
	*/

	void InitializeSDL(int width, int height);

	// text labels
	/*
	Text* nextPieceLabel;
	Text* scoreLabel;
	Text* scoreValue;
	Text* levelLabel;
	Text* levelValue;
	Text* linesLabel;
	Text* linesValue;
	Text* placedLabel;
	Text* placedValue;
	Text* gameOverText;
	Text* promptUserText;

	//Sounds
	Sound* music;
	Sound* rotateSound;

	int textWidth;
	int textHeight;*/
};

#endif