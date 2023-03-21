#pragma once
#include "Text.h"
#include "Button.h"

class SDLGame;

class MainMenu
{

public:
	MainMenu(SDLGame* app);
	~MainMenu();	
	void Update();
	void Draw();

private:
	SDLGame* app;

	TTF_Font* buttonFont = nullptr;
	TTF_Font* arial_72 = nullptr;

	Button* playButton = nullptr;	
	Button* quitButton = nullptr;

	Music* titleMusic = nullptr;
};