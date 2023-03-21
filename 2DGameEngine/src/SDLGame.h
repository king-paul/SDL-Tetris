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

class MainMenu;

enum GameState
{
	MAIN_MENU,
	RUNNING,
	PAUSED,
	GAME_OVER,
	QUIT
};

class SDLGame 
{
public:
	SDLGame(int width, int height);
	~SDLGame();
	int ticksLastFrame = 0;
	bool IsRunning() const;

	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	void StartGame();

	// getters
	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }
	bool MousePressed() { return leftMouseDown; }

	// setters
	void SetState(GameState state) { gameState = state; }

private:
	GameState gameState;
	SDL_Window* window;
	SDL_Renderer* renderer;	
	MainMenu* mainMenu;
	Tetris* game;

	// game objects	
	SDL_Rect tetromino[4];
	SDL_Rect nextTetromino[4];	 

	// panels
	SDL_Rect field = { GRID_OFFSET_X, GRID_OFFSET_Y, COLS * TILE_SIZE, ROWS * TILE_SIZE };
	SDL_Rect rightHandPanel{ WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT };
	SDL_Rect nextPieceBorder;	

	// input variables
	bool keyPressed = false;
	bool leftMouseDown = false;

	// drawing options
	bool showGrid = false;

	void InitializeSDL(int width, int height);

	// drawing functions
	void DrawBoard();
	void DrawTetromino(bool nextPiece);

	//void SetRenderColor(ColorName color);
	void SetRenderColor(SDL_Color color);
	void FadeLineDisplay();
	void DrawStats();

	// line display fading
	int alpha = 0;
	bool fadeIn = false;
	bool fadeCompleted = false;

	// fonts
	TTF_Font* arial_24;
	TTF_Font* arial_48;

	// text labels
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
	int textHeight;
};

#endif