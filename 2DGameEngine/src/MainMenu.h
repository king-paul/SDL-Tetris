#pragma once
#include "Text.h"
#include "Button.h"
#include "Sprite.h"

class SDLApp;

class MainMenu
{
public:
	MainMenu();
	~MainMenu();	
	void Update();
	void Draw();

private:	

	TTF_Font* buttonFont = nullptr;
	TTF_Font* regularFont = nullptr;

	Sprite* logo;
	Sprite* background;

	Text* version;
	Text* creator;

	Button* playButton = nullptr;	
	Button* quitButton = nullptr;

	Music* titleMusic = nullptr;
};