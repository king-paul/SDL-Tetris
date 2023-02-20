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

class Game 
{
private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;	
	Tetris* game;

	// game objects
	SDL_Rect border = { GRID_OFFSET_X, GRID_OFFSET_Y, COLS * TILE_SIZE, ROWS * TILE_SIZE };
	SDL_Rect tetromino[4]; 

	void DrawBoard();

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