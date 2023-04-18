#pragma once
#include "SDL_libraries.h"

enum class ColorName
{
	BLACK = 0,
	RED = 9,
	LIME = 10,
	BLUE = 12,
	YELLOW = 11,
	MAGENTA = 13,
	CYAN = 14,
	GRAY = 8,
	MAROON = 1,
	GREEN = 2,
	NAVY = 4,
	OLIVE = 3,
	PURPLE = 5,
	TEAL = 6,
	SILVER = 7,
	WHITE = 15
};

namespace Color
{
	const SDL_Color BLACK = { 0, 0, 0, 255 };
	const SDL_Color RED = { 255, 0, 0, 255 };
	const SDL_Color LIME = { 0, 255, 0, 255 };
	const SDL_Color BLUE = { 0, 0, 255, 255 };
	const SDL_Color YELLOW = { 255, 255, 0, 255 };
	const SDL_Color MAGENTA = { 255, 0, 255, 255 };
	const SDL_Color CYAN = { 0, 255, 255, 255 };
	const SDL_Color GRAY = { 128, 128, 128, 255 };
	const SDL_Color MAROON = { 128, 0, 0, 255 };
	const SDL_Color GREEN = { 0, 128, 0, 255 };
	const SDL_Color NAVY = { 0, 0, 128, 255 };
	const SDL_Color OLIVE = { 128, 128, 0, 255 };
	const SDL_Color PURPLE = { 128, 0, 128, 255 };
	const SDL_Color TEAL = { 0, 128, 128, 255 };
	const SDL_Color	SILVER = { 192, 192, 192, 255 };
	const SDL_Color WHITE = { 255, 255, 255, 255 };

	static SDL_Color GetColor(int number)
	{
		switch ((ColorName) number)
		{
			case ColorName::BLACK: return BLACK;
				break;
			case ColorName::RED: return RED;
				break;
			case ColorName::LIME: return LIME;
				break;
			case ColorName::BLUE: return BLUE;
				break;
			case ColorName::YELLOW: return YELLOW;
				break;
			case ColorName::MAGENTA: return MAGENTA;
				break;
			case ColorName::CYAN: return CYAN;
				break;
			case ColorName::GRAY: return GRAY;
				break;
			case ColorName::MAROON: return MAROON;
				break;
			case ColorName::GREEN: return GREEN;
				break;
			case ColorName::NAVY: return NAVY;
				break;
			case ColorName::OLIVE: return OLIVE;
				break;
			case ColorName::PURPLE: return PURPLE;
				break;
			case ColorName::TEAL: return TEAL;
				break;
			case ColorName::SILVER: return SILVER;
				break;
			case ColorName::WHITE: return WHITE;

			default: return { 0, 0, 0, 0 };
		}
	}
}