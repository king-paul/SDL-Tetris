#pragma once

#include <SDL.h>
#include <iostream>

#include "Constants.h"
#include "Colors.h"
#include "text.h"

#include "SDLGame.h"

class Button
{
public:
	Button(SDLGame* app, TTF_Font* font, int width, int height, int posX, int posY, const char* text,
		   SDL_Color backgroundColor = Color::SILVER, SDL_Color textColour = Color::BLACK) 
		 : m_renderer(app->GetRenderer()), m_window(app->GetWindow()), app(app)
	{
		// set background properties
		m_background.w = width;
		m_background.h = height;
		m_background.x = posX;
		m_background.y = posY;

		m_colorNormal = backgroundColor;

		m_text = new Text(m_renderer, font, textColour, text);
		m_text->SetStyle(TTF_STYLE_BOLD);

		// position text on center of button
		int paddingLeft = (m_background.w - m_text->GetWidth()) / 2;
		int paddingTop = (m_background.h - m_text->GetHeight()) / 2;
		m_text->SetPosition(m_background.x + paddingLeft, m_background.y + paddingTop);
	}

	~Button()
	{
		delete m_text;
	}

	void SetColours(SDL_Color normalColor, SDL_Color hoverColor, SDL_Color pressedColor)
	{
		m_colorNormal = normalColor;
		m_colourHover = hoverColor;
		m_colourPressed = pressedColor;
	}

	void Draw()
	{
		if (Pressed())
			SetRenderColor(m_colourPressed);
		else if (Rollover())
			SetRenderColor(m_colourHover);
		else
			SetRenderColor(m_colorNormal);
		SDL_RenderFillRect(m_renderer, &m_background); // draws the background

		m_text->Draw(); // renders the text
	}

	bool Rollover()
	{
		int x, y, globalX, globalY, windowX, windowY;

		int boundsLeft = m_background.x;
		int boundsRight = m_background.x + m_background.w;
		int boundsTop = m_background.y;
		int boundsBottom = m_background.y + m_background.h;

		SDL_GetGlobalMouseState(&globalX, &globalY);
		SDL_GetWindowPosition(m_window, &windowX, &windowY);
		SDL_GetMouseState(&x, &y); // position of mouse relative to window

		// check that cursor is on window
		if (globalX >= windowX && globalX <= windowX + WINDOW_WIDTH && globalY >= windowY && globalY <= windowY + WINDOW_HEIGHT)
		{
			// check that cursor is on button
			if (x >= boundsLeft && x <= boundsRight && y >= boundsTop && y <= boundsBottom)
				return true;
		}

		return false;
	}

	bool Pressed()
	{		
		mousePressed = app->MousePressed();
		return Rollover() && mousePressed;
	}

	bool Clicked()
	{		
		if (mousePressed && Rollover() && !app->MousePressed())
		{
			mousePressed = false;
			return true;
		}

		return false;
	}

private:

	SDLGame* app;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	SDL_Rect m_background;
	Text* m_text;
	
	// colours
	SDL_Color m_colorNormal;
	SDL_Color m_colourHover;
	SDL_Color m_colourPressed;

	bool mousePressed = false;

	void SetRenderColor(SDL_Color color)
	{
		SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	}

};