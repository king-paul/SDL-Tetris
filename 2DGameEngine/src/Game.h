#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Tetris.h"

const int TILE_SIZE = 32;
const int COLS = 12;
const int ROWS = 18;
const int GRID_OFFSET_X = 6;
const int GRID_OFFSET_Y = 12;

enum class Color
{
	BLACK = 0,
	RED = 9,
	LIME = 10,
	BLUE = 12,
	YELLOW = 11,
	MAGENTA = 13,
	CYAN = 14,
	GRAY = 8,
	MAROON = 1,
	GREEN = 2,
	NAVY = 4,
	OLIVE = 3,
	PURPLE = 5,
	TEAL = 6,
	WHITE = 15
};

class Game 
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

	void DrawBoard();
	void DrawCurrentPiece();

	void SetRenderColor(Color color);

public:	
	Game();
	~Game();
	int ticksLastFrame = 0;
	bool IsRunning() const;
	void Initialize(int width, int height);
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
};

#endif