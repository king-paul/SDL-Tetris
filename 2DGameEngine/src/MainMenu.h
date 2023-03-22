#pragma once
#include "Text.h"
#include "Button.h"
#include "Sprite.h"

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
	TTF_Font* regularFont = nullptr;

	//Text* title;
	Sprite* logo;
	Text* version;
	Text* creator;

	Button* playButton = nullptr;	
	Button* quitButton = nullptr;

	Music* titleMusic = nullptr;
};