#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "constants.h"

class Sprite
{
public:
	Sprite(const char* filename, SDL_Renderer* renderer) : m_renderer(renderer)
	{
		SDL_Surface* surface = IMG_Load(filename);		

		if (surface)
		{
			texture = SDL_CreateTextureFromSurface(renderer, surface);
			sourceRect = { 0, 0, surface->w , surface->h };
		}
		else
		{
			std::cerr << IMG_GetError() << std::endl;
		}

		SDL_FreeSurface(surface);
	}

	~Sprite()
	{
		
	}

	void Draw(SDL_Rect *destination)
	{
		SDL_RenderCopy(m_renderer, texture, &sourceRect, destination);
	}

	void Draw(int x, int y)
	{
		SDL_Rect destinationRect = { x, y, sourceRect.w , sourceRect.h };
		SDL_RenderCopy(m_renderer, texture, &sourceRect, &destinationRect);
	}

	void Draw(int x, int y, int width, int height)
	{
		SDL_Rect destinationRect = { x, y, width , height };
		SDL_RenderCopy(m_renderer, texture, &sourceRect, &destinationRect);
	}

private:
	SDL_Renderer* m_renderer;
	SDL_Rect sourceRect;
	SDL_Texture* texture;
};