#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Tetris.h"
#include "Text.h"
#include "Colors.h"

const int TILE_SIZE = 32;
const int COLS = 12;
const int ROWS = 18;
const int GRID_OFFSET_X = 6;
const int GRID_OFFSET_Y = 12;

class SDLGame 
{
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;	
	Tetris* game;

	// game objects
	//SDL_Rect border = { GRID_OFFSET_X, GRID_OFFSET_Y, COLS * TILE_SIZE, ROWS * TILE_SIZE };
	SDL_Rect tetromino[4]; 

	//keyboard pressed
	bool keyPressed = false;

	// drawing options
	bool showGrid = false;

	void InitializeSDL(int width, int height);

	// drawing functions
	void DrawBoard();
	void DrawCurrentPiece();

	//void SetRenderColor(ColorName color);
	void SetRenderColor(SDL_Color color);
	void FadeLineDisplay();
	void DrawStats();

	// line display fading
	int alpha = 0;
	bool fadeIn = false;
	bool fadeCompleted = false;

	// fonts & textures
	TTF_Font* arial;
	Text* scoreLabel;
	Text* scoreValue;

	int textWidth;
	int textHeight;

public:	
	SDLGame(int width, int height);
	~SDLGame();
	int ticksLastFrame = 0;
	bool IsRunning() const;	
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
};

#endif