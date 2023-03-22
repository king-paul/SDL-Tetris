#include "SDLGame.h"
#include "MainMenu.h"

MainMenu::MainMenu(SDLGame* app) : app(app)
{
	// load images
	logo = new Sprite("assets/images/title.png", app->GetRenderer());

	// load fonts
	buttonFont = TTF_OpenFont("assets/fonts/arial.ttf", 28);
	regularFont = TTF_OpenFont("assets/fonts/arial.ttf", 28);

	// create text
	version = new Text(app->GetRenderer(), buttonFont, Color::GREEN, "VERSION 0.5", 310, 190);
	version->SetStyle(TTF_STYLE_BOLD);

	creator = new Text(app->GetRenderer(), buttonFont, Color::RED, "Created By Paul King 2023", 230, 550);

	// create buttons
	playButton = new Button(app, regularFont, 300, 64, 250, 300, "PLAY GAME");
	playButton->SetColours(Color::LIME, Color::YELLOW, Color::GREEN);

	quitButton = new Button(app, regularFont, 300, 64, 250, 400, "QUIT");
	quitButton->SetColours(Color::RED, Color::YELLOW, Color::MAROON);

	titleMusic = new Music("assets/sounds/Title.mp3");
	titleMusic->Play();
}

MainMenu::~MainMenu()
{
	delete playButton;
	delete quitButton;

	TTF_CloseFont(buttonFont);
	TTF_CloseFont(regularFont);
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
	// sets the background colour
	app->SetRenderColor({ 50, 50, 50, 255 });
	SDL_RenderClear(app->GetRenderer());

	// show title logo
	logo->Draw(100, 50);

	// show text
	version->Draw();
	creator->Draw();

	// render buttons
	playButton->Draw();
	quitButton->Draw();
}