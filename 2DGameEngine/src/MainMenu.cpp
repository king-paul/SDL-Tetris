#include "SDLApp.h"
#include "MainMenu.h"

#include <string>

MainMenu::MainMenu()
{
	SDLApp& app = SDLApp::GetInstance();

	// load images
	background = new Sprite("assets/images/metal.jpg", app.GetRenderer());
	logo = new Sprite("assets/images/title.png", app.GetRenderer());

	// load fonts
	buttonFont = TTF_OpenFont("assets/fonts/arial.ttf", 28);
	regularFont = TTF_OpenFont("assets/fonts/arial.ttf", 28);
	pico = TTF_OpenFont("assets/fonts/pico.ttf", 24);
	charriot = TTF_OpenFont("assets/fonts/charriot.ttf", 48);	

	// create text
	char versionText[13];
	sprintf(versionText, "VERSION %.1f", VERSION);

	version = new Text(app.GetRenderer(), pico, Color::YELLOW, versionText, 280, 190);
	//version->SetStyle(TTF_STYLE_BOLD);

	creator = new Text(app.GetRenderer(), buttonFont, Color::RED, "Created By Paul King 2023", 230, 550);

	// create buttons
	playButton = new Button(regularFont, 300, 64, 250, 300, "PLAY GAME");
	playButton->SetColours(Color::LIME, Color::YELLOW, Color::GREEN);

	quitButton = new Button(regularFont, 300, 64, 250, 400, "QUIT");
	quitButton->SetColours(Color::RED, Color::YELLOW, Color::MAROON);

	titleMusic = new Music("assets/sounds/Title.mp3");
	titleMusic->Play(-1);
}

MainMenu::~MainMenu()
{
	delete playButton;
	delete quitButton;

	delete background;

	TTF_CloseFont(buttonFont);
	TTF_CloseFont(regularFont);
	TTF_CloseFont(pico);
	TTF_CloseFont(charriot);
}

void MainMenu::Update()
{
	SDLApp& app = SDLApp::GetInstance();

	if (playButton->Clicked())
	{
		app.StartGame();
	}

	if (quitButton->Clicked())
	{		
		app.SetState(GameState::QUIT);
	}
}

void MainMenu::Draw()
{
	SDLApp& app = SDLApp::GetInstance();

	// sets the background colour
	//app.SetRenderColor({ 50, 50, 50, 255 });
	
	SDL_RenderClear(app.GetRenderer());
	background->Draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// show title logo
	logo->Draw(100, 50);

	// show text
	version->Draw();
	creator->Draw();

	// render buttons
	playButton->Draw();
	quitButton->Draw();
}