#include "SDLGame.h"
#include "MainMenu.h"

MainMenu::MainMenu(SDLGame* app) : app(app)
{
	buttonFont = TTF_OpenFont("assets/fonts/arial.ttf", 14);

	// create buttons
	playButton = new Button(app, buttonFont, 150, 32, 320, 240, "Play Game");
	playButton->SetColours(Color::LIME, Color::YELLOW, Color::GREEN);

	quitButton = new Button(app, buttonFont, 150, 32, 320, 300, "Quit");
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
		app->StartGame();
	}

	if (quitButton->Clicked())
	{		
		app->SetState(GameState::QUIT);
	}
}

void MainMenu::Draw()
{
	// render buttons
	playButton->Draw();
	quitButton->Draw();
}