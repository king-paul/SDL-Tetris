#pragma once
#include "Text.h"
#include "Button.h"

class MainMenu
{

public:
	MainMenu(SDL_Renderer* renderer, SDL_Window* window);
	~MainMenu();	
	void Update();
	void Draw();

private:
	SDL_Renderer* m_renderer = nullptr;
	SDL_Window* m_window = nullptr;

	TTF_Font* buttonFont = nullptr;
	TTF_Font* arial_72 = nullptr;

	Button* playButton = nullptr;	
	Button* quitButton = nullptr;
};