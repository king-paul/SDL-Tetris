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
	void HandleInput(SDL_Keycode keycode);

private:

	void DrawInstructions();

	TTF_Font* regularFont = nullptr;
	TTF_Font* arial_16 = nullptr;
	TTF_Font* pico = nullptr;
	TTF_Font* charriot = nullptr;

	Sprite* logo;
	Sprite* background;

	Text* version;
	Text* creator;

	// instructrictions screen text
	Text* instructionsLabel;
	Text* controlsLabel;
	Text* scoringLabel;
	std::vector<Text*> instructions;
	std::vector<Text*> controls;
	std::vector<Text*> scoreValues;

	Button* playButton = nullptr;	
	Button* helpButton = nullptr;
	Button* quitButton = nullptr;
	Button* backButton = nullptr;

	Music* titleMusic = nullptr;

	bool showInstructions = false;
	bool playMusic = true;
};