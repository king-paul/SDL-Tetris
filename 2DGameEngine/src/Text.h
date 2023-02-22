#pragma once
#include <SDL_ttf.h>
#include "Colors.h"

class Text
{
private:
	SDL_Renderer* m_renderer;
	TTF_Font* m_font;
	SDL_Color m_colour;
	SDL_Surface* m_textLabel;
	SDL_Texture* m_texture;

	int m_xPos, m_yPos;
	const char* m_text;

	SDL_Rect m_textBox;

public:
	Text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color colour = Color::WHITE, int xPos = 0, int yPos = 0, const char* text = "")
		: m_renderer(renderer), m_font(font)
	{	
		m_colour = colour;
		m_textLabel = nullptr;
		m_texture = nullptr;
		m_xPos = xPos;
		m_yPos = yPos;
		m_text = text;

		/* initialize text component */
		
		m_textLabel = TTF_RenderText_Blended(m_font, text, m_colour); // creates surface		
		m_texture = SDL_CreateTextureFromSurface(m_renderer, m_textLabel); 
		// set position and scale of the texture
		int textWidth, textHeight;
		TTF_SizeText(m_font, text, &textWidth, &textHeight);
		m_textBox = { m_xPos, m_yPos, textWidth, textHeight };
	}

	~Text()
	{
		SDL_DestroyTexture(m_texture);
		SDL_FreeSurface(m_textLabel);
	}

	void Draw()
	{
		SDL_RenderCopy(m_renderer, m_texture, NULL, &m_textBox);
	}

	void Draw(const char* text)
	{
		SDL_DestroyTexture(m_texture);
		SDL_FreeSurface(m_textLabel);

		// create surface for text
		m_textLabel = TTF_RenderText_Blended(m_font, text, m_colour);
		// create texture from surface
		m_texture = SDL_CreateTextureFromSurface(m_renderer, m_textLabel);

		// set position and scale of the texture
		int textWidth, textHeight;		
		TTF_SizeText(m_font, text, &textWidth, &textHeight);
		m_textBox = { m_xPos, m_yPos, textWidth, textHeight };

		// render the texture
		SDL_RenderCopy(m_renderer, m_texture, NULL, &m_textBox);
	}

};