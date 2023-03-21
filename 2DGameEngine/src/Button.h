#pragma once

#include <SDL.h>
#include <iostream>

#include "Constants.h"
#include "Colors.h"
#include "text.h"

class Button
{
public:
	Button(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font, int width, int height, int posX, int posY, const char* text,
		SDL_Color backgroundColor = Color::SILVER, SDL_Color textColour = Color::BLACK) : m_renderer(renderer), m_window(window)
	{
		// set background properties
		m_background.w = width;
		m_background.h = height;
		m_background.x = posX;
		m_background.y = posY;

		m_colorNormal = backgroundColor;

		m_text = new Text(renderer, font, textColour, posX + width/4, posY + height/4, text);
		m_text->SetStyle(TTF_STYLE_BOLD);
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
		//CheckMousePress();

		if (Pressed())
			SetRenderColor(m_colourPressed);
		else if (Rollover())
			SetRenderColor(m_colourHover);
		else
			SetRenderColor(m_colorNormal);
		SDL_RenderFillRect(m_renderer, &m_background); // draws the background

		m_text->Draw(); // renders the text

		//CheckMousePress();
	}

	void Update(bool mousePressed)
	{

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
		if (Rollover())
		{
			SDL_Event event;
			SDL_PollEvent(&event);

			// checks if left mouse button is down
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			{				
				return true;
			}
		}

		return false;
	}

	bool Clicked()
	{
		if (Rollover())// && mousePressed)
		{
			SDL_Event event;
			SDL_PollEvent(&event);

			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{				
				mousePressed = false;
				return true;
			}
		}

		return false;
	}

	// updates the mousepressed variable
	void CheckMousePress()
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		if (mousePressed)
		{
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				mousePressed = false;
			}
		}
		else
		{
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
			{
				mousePressed = true;
			}
		}
	}

private:
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