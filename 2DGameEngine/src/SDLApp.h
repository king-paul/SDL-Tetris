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

	static SDLApp& GetInstance();

	bool IsRunning() const;

	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	void StartGame();
	void EndGame();

	// getters
	SDL_Window* GetWindow() { return window; }
	SDL_Renderer* GetRenderer() { return renderer; }
	bool KeyPressed() { return keyPressed; }
	bool MousePressed() { return leftMouseDown; }

	// setters
	void SetState(GameState state) { gameState = state; }
	//void SetRenderColor(ColorName color);
	void SetRenderColor(SDL_Color color);

	// getters
	GameState GetState() { return gameState; }

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

	void InitializeSDL(int width, int height);
};

#endif