#include "MainMenu.h"

MainMenu::MainMenu(SDL_Renderer* renderer, SDL_Window* window) : m_renderer(renderer), m_window(window)
{
	buttonFont = TTF_OpenFont("assets/fonts/arial.ttf", 14);

	// create buttons
	playButton = new Button(renderer, window, buttonFont, 150, 32, 320, 240, "Play Game");
	playButton->SetColours(Color::LIME, Color::YELLOW, Color::GREEN);

	quitButton = new Button(renderer, window, buttonFont, 150, 32, 320, 300, "Quit");
	quitButton->SetColours(Color::RED, Color::YELLOW, Color::MAROON);
}

MainMenu::~MainMenu()
{
	delete playButton;
	delete quitButton;

	TTF_CloseFont(buttonFont);
}

void MainMenu::Update()
{
	if (playButton->Clicked())
	{
		std::cout << "You clicked the Play Game button" << std::endl;
	}

	if (quitButton->Clicked())
	{
		std::cout << "You clicked the Quit button" << std::endl;
	}
}

void MainMenu::Draw()
{
	// render buttons
	playButton->Draw();
	quitButton->Draw();
}