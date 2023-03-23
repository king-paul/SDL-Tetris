#pragma once
#include <SDL.h>
#include <map>
#include <string>

#include "Tetris.h"
#include "Text.h"
#include "Sprite.h"
#include "Sound.h"

class SDLApp;

class GameScreen
{
public:
	GameScreen();
	~GameScreen();

	void Update(float deltaTime);
	void Render();

	void HandleInput(SDL_Keycode keypressed);

private:	
	Tetris* game;

	SDL_Renderer* renderer;

	// game objects	
	SDL_Rect tetromino[4];
	SDL_Rect nextTetromino[4];

	// panels
	SDL_Rect field = { GRID_OFFSET_X, GRID_OFFSET_Y, COLS * TILE_SIZE, ROWS * TILE_SIZE };
	SDL_Rect rightHandPanel{ WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT };
	SDL_Rect nextPieceBorder;

	std::map<string, Sprite*> blocks;
	std::map<string, Text*> textLabels;

	// block sprites
	/*Sprite* blockBlue;
	Sprite* blockCyan;
	Sprite* blockGreen;
	Sprite* blockOrange;
	Sprite* blockPurple;
	Sprite* blockRed;
	Sprite* blockYellow;*/

	Sound* music;
	Sound* rotateSound;

	// drawing options
	bool showGrid = false;

	// line display fading
	int alpha = 0;
	bool fadeIn = false;
	bool fadeCompleted = false;

	// fonts
	TTF_Font* arial_24;
	TTF_Font* arial_48;

	// functions	
	
	// drawing functions
	void DrawBoard();
	void DrawTetromino(bool nextPiece);

	void FadeLineDisplay();
	void DrawStats();

};