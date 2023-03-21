#include "SDLGame.h"
#include "MainMenu.h"

MainMenu::MainMenu(SDLGame* app) : app(app)
{
	buttonFont = TTF_OpenFont("assets/fonts/arial.ttf", 14);
	titleFont = TTF_OpenFont("assets/fonts/arial.ttf", 72);

	title = new Text(app->GetRenderer(), titleFont, Color::GREEN, 240, 120, "PAULTRIS");
	title->SetStyle(TTF_STYLE_BOLD);

	// create buttons
	playButton = new Button(app, buttonFont, 150, 32, 320, 320, "Play Game");
	playButton->SetColours(Color::LIME, Color::YELLOW, Color::GREEN);

	quitButton = new Button(app, buttonFont, 150, 32, 320, 380, "Quit");
	quitButton->SetColours(Color::RED, Color::YELLOW, Color::MAROON);

	titleMusic = new Music("assets/sounds/Title.mp3");
	titleMusic->Play();
}

MainMenu::~MainMenu()
{
	delete playButton;
	delete quitButton;

	TTF_CloseFont(buttonFont);
	TTF_CloseFont(titleFont);
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
	// show heading
	title->Draw();

	// render buttons
	playButton->Draw();
	quitButton->Draw();
}