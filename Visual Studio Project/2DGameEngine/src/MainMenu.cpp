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
	arial_16 = TTF_OpenFont("assets/fonts/arial.ttf", 16);
	regularFont = TTF_OpenFont("assets/fonts/arial.ttf", 28);
	pico = TTF_OpenFont("assets/fonts/pico.ttf", 24);
	charriot = TTF_OpenFont("assets/fonts/charriot.ttf", 48);	

	// create text
	char versionText[13];
	sprintf(versionText, "VERSION %.1f", VERSION);

	version = new Text(app.GetRenderer(), pico, Color::YELLOW, versionText, 280, 190);
	//version->SetStyle(TTF_STYLE_BOLD);

	creator = new Text(app.GetRenderer(), regularFont, Color::RED, "Created By Paul King 2023", 230, 550);

	// create buttons
	playButton = new Button(regularFont, 300, 64, 250, 250, "PLAY GAME");
	playButton->SetColours(Color::LIME, Color::YELLOW, Color::GREEN);	

	helpButton = new Button(regularFont, 300, 64, 250, 350, "INSTRUCTIONS");
	helpButton->SetColours(Color::BLUE, Color::CYAN, Color::NAVY);

	quitButton = new Button(regularFont, 300, 64, 250, 450, "QUIT");
	quitButton->SetColours(Color::RED, Color::YELLOW, Color::MAROON);

	backButton = new Button(arial_16, 200, 40, 300, 550, "BACK TO MAIN MENU");
	backButton->SetColours(Color::RED, Color::YELLOW, Color::MAROON);

	// load music
	titleMusic = new Music("assets/sounds/Title.mp3");

	if(playMusic)
		titleMusic->Play(-1);	

	// Instructions screen
	instructionsLabel = new Text(app.GetRenderer(), regularFont, Color::RED, 300, 10, "HOW TO PLAY", TTF_STYLE_BOLD);
	scoringLabel = new Text(app.GetRenderer(), regularFont, Color::LIME, 50, 175, "SCORING", TTF_STYLE_BOLD);
	controlsLabel = new Text(app.GetRenderer(), regularFont, Color::YELLOW, 50, 300, "CONTROLS", TTF_STYLE_BOLD);

	instructions.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"The objective of the game is to score as many points as possible by clearing horizontal lines"));
	instructions.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"of Blocks. The player must rotate, move, and drop the falling tetriminos inside the playing"));
	instructions.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"field. Lines are cleared when they are filled with Blocks and have no empty spaces."));
	instructions.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		""));
	instructions.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"As lines are cleared, the level increases and tetriminos fall faster, making the game progressively"));
	instructions.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"more challenging. If the Blocks land above the top of the playing field, the game is over."));	

	scoreValues.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE, "SINGLE LINE: 40 points * Level"));
	scoreValues.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE, "2 LINES: 100 points * Level"));
	scoreValues.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE, "3 LINES: 300 points * Level"));
	scoreValues.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE, "4 LINES: 1200 points * Level"));

	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"LEFT ARROW or A: Move tetromino to the left"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"RIGHT ARROW or D: Move tetromino to the right"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"UP ARROW or S: Move tetromino downward"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"UP ARROW or W or E: Rotate Tetromino clockwise"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"Q: Rotate Tetromino Anti-clockwise"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"SPACEBAR: Drop tetromino instanstly"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"G: Toggle grid view"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"ESC: Open/Close pause menu"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"M: Toggle Music"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"=: Turn volume Up"));
	controls.push_back(new Text(app.GetRenderer(), arial_16, Color::WHITE,
		"-: Turn Volume Down"));
}

MainMenu::~MainMenu()
{
	delete playButton, helpButton, quitButton;

	for (Text* line : instructions)	
		delete line;	
	instructions.clear();

	for (Text* line : scoreValues)	
		delete line;	
	scoreValues.clear();

	for (Text* line : controls)	
		delete line;	
	controls.clear();

	delete instructionsLabel, scoringLabel, controlsLabel;

	delete background;

	TTF_CloseFont(arial_16);
	TTF_CloseFont(regularFont);
	TTF_CloseFont(pico);
	TTF_CloseFont(charriot);
}

void MainMenu::Update()
{
	SDLApp& app = SDLApp::GetInstance();

	if (!showInstructions)
	{
		if (playButton->Clicked())
		{
			app.StartGame();
		}

		if (helpButton->Clicked())
		{
			showInstructions = true;
		}

		if (quitButton->Clicked())
		{
			app.SetState(GameState::QUIT);
		}
	}
	else
	{
		if (backButton->Clicked())
		{
			showInstructions = false;
		}
	}

}

void MainMenu::Draw()
{
	SDLApp& app = SDLApp::GetInstance();

	// sets the background colour
	app.SetRenderColor({ 50, 50, 50, 255 });	
	SDL_RenderClear(app.GetRenderer());	

	if (showInstructions)
	{
		DrawInstructions();
	}
	else
	{
		background->Draw(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		// show title logo
		logo->Draw(100, 50);

		// show text
		version->Draw();
		creator->Draw();

		// render buttons
		playButton->Draw();
		helpButton->Draw();
		quitButton->Draw();
	}
}

void MainMenu::HandleInput(SDL_Keycode keycode)
{
	if (keycode == SDLK_m)
	{
		playMusic = !playMusic;

		if (playMusic)
			titleMusic->Play(-1);
		else
			titleMusic->Stop();
	}

}

void MainMenu::DrawInstructions()
{
	instructionsLabel->Draw();
	scoringLabel->Draw();
	controlsLabel->Draw();

	int initialSpace = TTF_FontLineSkip(regularFont);
	int lineSpacing = TTF_FontLineSkip(arial_16);

	for (int i=0; i< instructions.size(); i++)
	{
		instructions[i]->SetPosition(50, instructionsLabel->GetRect().y +
			initialSpace + lineSpacing * i);
		instructions[i]->Draw();
	}

	for (int i = 0; i < scoreValues.size(); i++)
	{
		scoreValues[i]->SetPosition(50, scoringLabel->GetRect().y +
			initialSpace + lineSpacing * i);
		scoreValues[i]->Draw();
	}

	for (int i = 0; i < controls.size(); i++)
	{
		controls[i]->SetPosition(50, controlsLabel->GetRect().y + 
			initialSpace + lineSpacing * i);
		controls[i]->Draw();
	}

	backButton->Draw();
}