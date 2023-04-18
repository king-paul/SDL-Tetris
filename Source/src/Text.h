#pragma once
#include "SDL_libraries.h"
#include "Colors.h"

class Text
{
protected:
	SDL_Renderer* m_renderer;
	TTF_Font* m_font;
	SDL_Color m_colour;
	SDL_Surface* m_textLabel;
	SDL_Texture* m_texture;

	int m_xPos, m_yPos;
	int m_style;
	const char* m_text;

	SDL_Rect m_textBox;

	void RefreshText(const char* text)
	{
		SDL_DestroyTexture(m_texture);
		SDL_FreeSurface(m_textLabel);

		TTF_SetFontStyle(m_font, m_style);

		// create surface for text
		m_textLabel = TTF_RenderText_Blended_Wrapped(m_font, text, m_colour, 600);
		// create texture from surface
		m_texture = SDL_CreateTextureFromSurface(m_renderer, m_textLabel);

		// set position and scale of the texture
		int textWidth, textHeight;
		TTF_SizeText(m_font, text, &textWidth, &textHeight);
		m_textBox = { m_xPos, m_yPos, textWidth, textHeight };
	}

public:
	Text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color colour = Color::WHITE, const char* text = "", 
		 int xPos = 0, int yPos = 0, int style = TTF_STYLE_NORMAL)
		: m_renderer(renderer), m_font(font), m_style(style)
	{	
		m_colour = colour;
		m_textLabel = nullptr;
		m_texture = nullptr;
		m_xPos = xPos;
		m_yPos = yPos;
		m_text = text;

		TTF_SetFontStyle(m_font, m_style);

		/* initialize text component */		
		//m_textLabel = TTF_RenderText_Blended(m_font, text, m_colour); // creates surface
		m_textLabel = TTF_RenderText_Blended(m_font, text, m_colour);
		m_texture = SDL_CreateTextureFromSurface(m_renderer, m_textLabel);

		// set position and scale of the texture
		int textWidth, textHeight;
		TTF_SizeText(m_font, text, &textWidth, &textHeight);		

		m_textBox = { m_xPos, m_yPos, textWidth, textHeight };		
		
	}

	Text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color colour = Color::WHITE, int xPos = 0, int yPos = 0, const char* text = "",
		int style = TTF_STYLE_NORMAL)
		: m_renderer(renderer), m_font(font), m_style(style)
	{
		m_colour = colour;
		m_textLabel = nullptr;
		m_texture = nullptr;
		m_xPos = xPos;
		m_yPos = yPos;
		m_text = text;

		TTF_SetFontStyle(m_font, m_style);

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

	// getters

	int GetWidth() { return m_textLabel->w; }
	int GetHeight() { return m_textLabel->h; }
	SDL_Rect GetRect() { return m_textBox; }

	// setters

	void SetColour(SDL_Color colour) {
		m_colour = colour;
		RefreshText(m_text);
	}
	
	void SetStyle(int style)
	{
		m_style = style;
		RefreshText(m_text);
	}

	void SetPosition(int x, int y)
	{
		m_xPos = x;
		m_yPos = y;
		m_textBox.x = x;
		m_textBox.y = y;
	}

	// draw functions

	virtual void Draw()
	{
		TTF_SetFontStyle(m_font, m_style);
		SDL_RenderCopy(m_renderer, m_texture, NULL, &m_textBox);
	}

	virtual void Draw(const char* text)
	{
		RefreshText(text);

		// render the texture
		SDL_RenderCopy(m_renderer, m_texture, NULL, &m_textBox);
	}	
	
};

class TextBox : public Text
{
private:
	SDL_Color m_bgColor;
	SDL_Rect m_background;

public:
	TextBox(SDL_Renderer* renderer, TTF_Font* font, SDL_Color foreground, SDL_Color background, const char* text = "", 
		    int xPos = 0, int yPos = 0, int width = 100, int hPadding = 10, int vPadding = 0)
		: Text(renderer, font, foreground, text, xPos, yPos), m_bgColor(background)
	{
		m_background.w = width + hPadding * 2;
		m_background.h = m_textBox.h + vPadding * 2;
		m_background.x = m_textBox.x - hPadding;
		m_background.y = m_textBox.y - vPadding;
	}

	TextBox(SDL_Renderer* renderer, TTF_Font* font, int xPos = 0, int yPos = 0, SDL_Color foreground = Color::WHITE, 
			SDL_Color background = Color::BLACK, int width = 100, int hPadding = 10, int vPadding = 2, const char* text = "")
		: Text(renderer, font, foreground, text, xPos, yPos), m_bgColor(background)
	{
		m_background.w = width + hPadding * 2;
		m_background.h = m_textBox.h + vPadding * 2;
		m_background.x = m_textBox.x - hPadding;
		m_background.y = m_textBox.y - vPadding;
	}

	void Draw() override
	{		
		SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, m_bgColor.a);
		SDL_RenderFillRect(m_renderer, &m_background);
		SDL_SetRenderDrawColor(m_renderer, Color::GRAY.r, Color::GRAY.g, Color::GRAY.b, Color::GRAY.a);
		SDL_RenderDrawRect(m_renderer, &m_background);
		Text::Draw();
	}

	virtual void Draw(const char* text) override
	{		
		SDL_SetRenderDrawColor(m_renderer, m_bgColor.r, m_bgColor.g, m_bgColor.b, 255);
		SDL_RenderFillRect(m_renderer, &m_background);
		SDL_SetRenderDrawColor(m_renderer, Color::GRAY.r, Color::GRAY.g, Color::GRAY.b, Color::GRAY.a);
		SDL_RenderDrawRect(m_renderer, &m_background);
		Text::Draw(text);
	}
};